#include <string.h>

#include "thread_assist_pair.hpp"
#include "Weave/Core/WeaveGlobals.h"
#include "Weave/Profiles/device-control/DeviceControl.h"
#include "Weave/Support/CodeUtils.h"
#include "openthread/openthread-freertos.h"

#include <task.h>

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

ThreadAssistedPairer::ThreadAssistedPairer(TaskHandle_t aTask)
    : mNowScanCnt(0), mTask(aTask){};

WEAVE_ERROR ThreadAssistedPairer::StartThreadAssistedPairing()
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    printf("Start scan thread networks\n");
    SuccessOrExit(err = ScanThreadNetworks());
    
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    mNowConnectId = 0;
    err = StartPair();
exit:
    return err;
}

WEAVE_ERROR ThreadAssistedPairer::ScanThreadNetworks()
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;
    otError     otErr;

    OT_API_CALL(err = otIp6SetEnabled(otrGetInstance(), true));
    if (otErr != OT_ERROR_NONE)
    {
        ExitNow(err = WEAVE_ERROR_INCORRECT_STATE);
    }
    mNowScanCnt = 0;
    OT_API_CALL(otErr = otThreadDiscover(otrGetInstance(), 0, 0xffff, true, false, sOtScanHandler, this));
    //OT_API_CALL(otErr = otLinkActiveScan(otxGetInstance(), 0, 0, sOtScanHandler, this));
    if (otErr != OT_ERROR_NONE)
    {
        ExitNow(err = WEAVE_ERROR_INCORRECT_STATE);
    }
exit:
    return err;
}

void ThreadAssistedPairer::sOtScanHandler(otActiveScanResult *result, void *ctx)
{
    ThreadAssistedPairer *p = static_cast<ThreadAssistedPairer *>(ctx);
    p->OtScanHandler(result);
}

void ThreadAssistedPairer::OtScanHandler(otActiveScanResult *result)
{
    if (result == NULL)
    {
        printf("Scan done\n");
        printf("Scan gives %d networks\n", mNowScanCnt);
        xTaskNotifyGive(mTask);
    }
    else if (mNowScanCnt < MAX_SCAN_RESULTS_CNT)
    {
        printf("Found network %s\n", result->mNetworkName.m8);
        memcpy(&mScanResults[mNowScanCnt], result, sizeof(otActiveScanResult));
        mNowScanCnt++;
    }
    else
    {
        printf("Discard scan result\n");
    }
}

WEAVE_ERROR ThreadAssistedPairer::StartPair()
{
    WEAVE_ERROR err = WEAVE_ERROR_NOT_CONNECTED;
    for (; mNowConnectId < mNowScanCnt; mNowConnectId++)
    {
        WEAVE_ERROR err;
        otError     otErr;

        printf("Start thread connection to %s\n", mScanResults[mNowConnectId].mNetworkName.m8);
        otErr = ConnectToPan(&mScanResults[mNowConnectId]);
        if (otErr != OT_ERROR_NONE)
        {
            continue;
        }

        printf("Open unsecure port\n");
        otErr = OpenUnsecurePort();
        if (otErr != OT_ERROR_NONE)
        {
            continue;
        }

        err = ConnectToWeave(&mScanResults[mNowConnectId]);
        if (err == WEAVE_NO_ERROR)
        {
            break;
        }
    }

    return err;
}

otError ThreadAssistedPairer::ConnectToPan(otActiveScanResult *aTarget)
{
    otError err;
    otLinkModeConfig linkConfig;

    OT_API_CALL(err = otIp6SetEnabled(otrGetInstance(), false));
    SuccessOrExit(err);

    OT_API_CALL(err = otThreadSetNetworkName(otrGetInstance(), aTarget->mNetworkName.m8));
    SuccessOrExit(err);

    OT_API_CALL(err = otLinkSetPanId(otrGetInstance(), aTarget->mPanId));
    SuccessOrExit(err);

    OT_API_CALL(err = otThreadSetExtendedPanId(otrGetInstance(), &aTarget->mExtendedPanId));
    SuccessOrExit(err);

    OT_API_CALL(err = otLinkSetChannel(otrGetInstance(), aTarget->mChannel));
    SuccessOrExit(err);

    OT_API_CALL(err = otIp6SetEnabled(otrGetInstance(), true));
    SuccessOrExit(err);

    linkConfig.mRxOnWhenIdle = true;
    linkConfig.mDeviceType = false;
    linkConfig.mNetworkData = true;
    linkConfig.mSecureDataRequests = true;

    OT_API_CALL(err = otThreadSetLinkMode(otrGetInstance(), linkConfig));
    SuccessOrExit(err);
exit:
    return err;
}

otError ThreadAssistedPairer::OpenUnsecurePort()
{
    otError err;

    OT_API_CALL(err = otIp6AddUnsecurePort(otrGetInstance(), WEAVE_UNSECURE_PORT));
    return err;
};

WEAVE_ERROR ThreadAssistedPairer::ConnectToWeave(otActiveScanResult *aTarget)
{
    otExtAddress extAddr = aTarget->mExtAddress;
    ip6_addr_t   peerBufAddr;
    IPAddress    peerAddr;
    uint64_t     peerNodeId;
    uint8_t *    addrPtr = reinterpret_cast<uint8_t*>(&peerBufAddr.addr);

    // weave node id is ext addr in big endian
    peerNodeId = 0;
    for (size_t i = 1; i < sizeof(peerNodeId); i++)
    {
        peerNodeId = peerNodeId << 8;
        peerNodeId |= extAddr.m8[i];
    }

    // local address bit mask
    extAddr.m8[0] ^= 0x02;
    memset(peerBufAddr.addr, 0, sizeof(peerBufAddr.addr));
    addrPtr[0] = 0xfe;
    addrPtr[1] = 0x80;
    memcpy(&addrPtr[8], extAddr.m8, sizeof(extAddr.m8));
    peerBufAddr.zone = IP6_NO_ZONE;
    peerAddr = IPAddress::FromIPv6(peerBufAddr);

    printf("%08x\n", peerAddr.Addr[0]);
    char buf[100];
    peerAddr.ToString(buf, 100);
    printf("peer addr %s\n", buf);

    // connection callback
    mWeaveConnection                       = MessageLayer.NewConnection();
    mWeaveConnection->AppState             = this;
    mWeaveConnection->OnConnectionComplete = sWeaveConnectedHandler;
    mWeaveConnection->OnConnectionClosed   = sWeaveConnectionCloseHandler;
    // start connection
    return mWeaveConnection->Connect(peerNodeId, nl::Weave::kWeaveAuthMode_None, peerAddr, WEAVE_UNSECURE_PORT);
}

void ThreadAssistedPairer::sWeaveConnectedHandler(WeaveConnection *aConnection, WEAVE_ERROR aError)
{
    ThreadAssistedPairer *pairer = static_cast<ThreadAssistedPairer *>(aConnection->AppState);
    pairer->WeaveConnectedHandler(aConnection, aError);
}

void ThreadAssistedPairer::WeaveConnectedHandler(WeaveConnection *aConnection, WEAVE_ERROR aError)
{
    printf("Weave connected\n");
    if (aError != WEAVE_NO_ERROR)
    {
        aConnection->Abort();
        mNowConnectId++;
        StartPair();
    }
    else
    {
        printf("Start PASE\n");
        StartPASE();
    }
}

void ThreadAssistedPairer::sWeaveConnectionCloseHandler(WeaveConnection *aConnection, WEAVE_ERROR aError)
{
    ThreadAssistedPairer *pairer = static_cast<ThreadAssistedPairer *>(aConnection->AppState);
    pairer->WeaveConectionCloseHandler(aConnection, aError);
}

void ThreadAssistedPairer::WeaveConectionCloseHandler(WeaveConnection *aConnection, WEAVE_ERROR aError)
{
    // Whether unexpected or user-intiated, connection close indicates PASE failure
    //
    mNowConnectId++;
    StartPair();
}

WEAVE_ERROR ThreadAssistedPairer::StartPASE()
{
    WEAVE_ERROR      err = WEAVE_NO_ERROR;
    printf("New exchange mgr\n");
    ExchangeContext *ec  = ExchangeMgr.NewContext(mWeaveConnection, NULL);

    SecurityMgr.OnSessionEstablished = sPASEEstablishedHandler;
    SecurityMgr.OnSessionError       = sPASEEFailedHandler;

    ExchangeMgr.AllowUnsolicitedMessages(mWeaveConnection);
    VerifyOrExit(ec != NULL, err = WEAVE_ERROR_NO_MEMORY);
    ec->PeerNodeId = kAnyNodeId;
    printf("Send looking to rendezvous\n");
    err            = nl::Weave::Profiles::DeviceControl::SendLookingToRendezvous(ec);

exit:
    if (ec != NULL)
    {
        ec->Close();
    }
    return err;
}

void ThreadAssistedPairer::sPASEEstablishedHandler(WeaveSecurityManager *sm,
                                                   WeaveConnection *     con,
                                                   void *                reqState,
                                                   uint16_t              sessionKeyId,
                                                   uint64_t              peerNodeId,
                                                   uint8_t               encType)
{
    ThreadAssistedPairer *pairer = static_cast<ThreadAssistedPairer *>(con->AppState);
    pairer->PASEEstablishedHandler(sm, con, reqState, sessionKeyId, peerNodeId, encType);
}

void ThreadAssistedPairer::PASEEstablishedHandler(WeaveSecurityManager *sm,
                                                  WeaveConnection *     con,
                                                  void *                reqState,
                                                  uint16_t              sessionKeyId,
                                                  uint64_t              peerNodeId,
                                                  uint8_t               encType)
{
    // log output
    printf("Successfully established pased session\n");

    // hand off the connection
    con->AppState             = NULL;
    con->OnConnectionClosed   = NULL;
    con->OnConnectionComplete = NULL;
    mWeaveConnection          = NULL;
}

void ThreadAssistedPairer::sPASEEFailedHandler(WeaveSecurityManager *sm,
                                               WeaveConnection *     con,
                                               void *                reqState,
                                               WEAVE_ERROR           localErr,
                                               uint64_t              peerNodeId,
                                               StatusReport *        statusReport)
{
    ThreadAssistedPairer *pairer = static_cast<ThreadAssistedPairer *>(con->AppState);
    pairer->PASEEFailedHandler(sm, con, reqState, localErr, peerNodeId, statusReport);
}

void ThreadAssistedPairer::PASEEFailedHandler(WeaveSecurityManager *sm,
                                              WeaveConnection *     con,
                                              void *                reqState,
                                              WEAVE_ERROR           localErr,
                                              uint64_t              peerNodeId,
                                              StatusReport *        statusReport)
{
    printf("PASE failed\n");

    mWeaveConnection->Shutdown();
    mNowConnectId++;
    StartPair();
}

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

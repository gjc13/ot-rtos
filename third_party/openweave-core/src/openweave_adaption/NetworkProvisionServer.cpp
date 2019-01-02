#include "Weave/Profiles/WeaveProfiles.h"
#include "openthread/link.h"
#include "openthread/openthread-freertos.h"
#include "openthread/thread.h"
#include "openweave_adaption/NetworkProvisionServer.hpp"

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

using namespace nl::Weave::Profiles::NetworkProvisioning;
using namespace nl::Weave::Profiles;

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::Init(WeaveExchangeManager *exchangeMgr)
{
    WEAVE_ERROR err;

    err = this->NetworkProvisioningServer::Init(exchangeMgr);
    SuccessOrExit(err);

    mScannedNetworks         = NULL;
    mScannedNetworksSize     = 0;
    mScannedNetworksCapacity = 0;
    for (size_t i = 0; i < kmMaxProvisionSize; i++)
    {
        mProvisionNetworks->Clear();
        mProvisionNetworks[i].ThreadPANId = 0xffff;
    }

    SetDelegate(this);
exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleScanNetworks(uint8_t networkType)
{
    WEAVE_ERROR err;

    // Log first
    {
        char ipAddrStr[64];
        mCurOp->PeerAddr.ToString(ipAddrStr, sizeof(ipAddrStr));
        printf("ScanNetworks request received from node %" PRIX64 " (%s)\n", mCurOp->PeerNodeId, ipAddrStr);
        printf("  Requested Network Type: %d\n", networkType);
    }

    if (networkType == kNetworkType_Thread)
    {
        err = ScanThreadNetworks();
    }
    else
    {
        err = SendStatusReport(WeaveProfileId::kWeaveProfile_NetworkProvisioning, kStatusCode_UnsupportedNetworkType);
    }

    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::ScanThreadNetworks()
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    uint32_t scanChannels = 0;
    uint16_t scanDuration = 0;

    mScannedNetworks         = new NetworkInfo[kmDefaultCapacity];
    mScannedNetworksCapacity = kmDefaultCapacity;
    mScannedNetworksSize     = 0;

    if (mScannedNetworks == NULL)
    {
        err = SendScannedNetworks();
    }
    else
    {
        otError otErr;
        OT_API_CALL(otErr = otLinkActiveScan(otrGetInstance(), scanChannels, scanDuration, sOtScanHandler, this));

        if (otErr == OT_ERROR_BUSY)
        {
            err = SendStatusReport(kWeaveProfile_Common, kStatusCode_InvalidState, err);
        }
    }

    return err;
}

void NetworkProvisioningFreeRTOSServer::sOtScanHandler(otActiveScanResult *result, void *ctx)
{
    NetworkProvisioningFreeRTOSServer *server = static_cast<NetworkProvisioningFreeRTOSServer *>(ctx);
    server->otScanHandler(result);
}

void NetworkProvisioningFreeRTOSServer::otScanHandler(otActiveScanResult *result)
{
    if (result == NULL)
    {
        if (mScannedNetworks)
        {
            SendScannedNetworks();
        }
    }
    else if (result->mIsJoinable && mScannedNetworks)
    {
        if (mScannedNetworksSize == mScannedNetworksCapacity)
        {
            NetworkInfo *mNewBuffer = new NetworkInfo[mScannedNetworksCapacity * 2];
            if (mNewBuffer == NULL)
            {
                SendScannedNetworks();
            }
            else
            {
                memcpy(mNewBuffer, mScannedNetworks, mScannedNetworksSize * sizeof(NetworkInfo));
                delete[] mScannedNetworks;
                mScannedNetworks = mNewBuffer;
                mScannedNetworksCapacity *= 2;
            }
        }
        NetworkInfo &info        = mScannedNetworks[mScannedNetworksSize];
        info.NetworkId           = -1;
        info.ThreadPANId         = result->mPanId;
        info.ThreadExtendedPANId = static_cast<uint8_t *>(malloc(sizeof(result->mExtendedPanId.m8)));
        memcpy(info.ThreadExtendedPANId, result->mExtendedPanId.m8, sizeof(result->mExtendedPanId.m8));
        info.ThreadChannel     = result->mChannel;
        info.ThreadNetworkName = static_cast<char *>(malloc(sizeof(result->mNetworkName.m8)));
        memcpy(info.ThreadNetworkName, result->mNetworkName.m8, sizeof(result->mNetworkName.m8));

        for (size_t i = 0; i < kmMaxProvisionSize; i++)
        {
            if (info.ThreadPANId == mProvisionNetworks[i].ThreadPANId &&
                strcmp(info.ThreadNetworkName, mProvisionNetworks[i].ThreadNetworkName) == 0 &&
                memcmp(info.ThreadExtendedPANId, mProvisionNetworks[i].ThreadExtendedPANId,
                       sizeof(result->mExtendedPanId.m8)) == 0)
            {
                info.NetworkId = mProvisionNetworks[i].NetworkId;
            }
        }

        mScannedNetworksSize++;
    }
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::SendScannedNetworks()
{
    WEAVE_ERROR   err     = WEAVE_NO_ERROR;
    PacketBuffer *respBuf = PacketBuffer::New();
    TLVWriter     writer;

    VerifyOrExit(respBuf != NULL, err = WEAVE_ERROR_NO_MEMORY);
    writer.Init(respBuf);

    err = NetworkInfo::EncodeList(writer, mScannedNetworksSize, mScannedNetworks, kNetworkType_Thread,
                                  NetworkInfo::kEncodeFlag_All, mScannedNetworksSize);
    SuccessOrExit(err);

    err = writer.Finalize();
    SuccessOrExit(err);

    err     = SendNetworkScanComplete(mScannedNetworksSize, respBuf);
    respBuf = NULL;
    SuccessOrExit(err);

exit:
    if (respBuf != NULL)
        PacketBuffer::Free(respBuf);

    delete[] mScannedNetworks;
    mScannedNetworks         = NULL;
    mScannedNetworksSize     = 0;
    mScannedNetworksCapacity = 0;
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::ValidateNetworkConfig(NetworkInfo &netConfig)
{
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    if (netConfig.NetworkType == kNetworkType_NotSpecified)
    {
        printf("Invalid network configuration: network type not specified\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
        SuccessOrExit(err);
        ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
    }

    if (netConfig.NetworkType == kNetworkType_WiFi)
    {
        printf("Invalid network configuration: wifi not supported\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
        SuccessOrExit(err);
        ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
    }

    if (netConfig.NetworkType == kNetworkType_Thread)
    {
        // Verify that other network parameters are valid when Extended PAN Id (EPANID) present.
        // When EPANID is specified: new Thread network should be added.
        // When EPANID is not specified: new Thread network should be created.
        if (netConfig.ThreadExtendedPANId != NULL)
        {
            if (netConfig.ThreadNetworkName == NULL)
            {
                printf("Invalid network configuration: Missing Thread network name\n");
                err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
                SuccessOrExit(err);
                ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
            }

            if (netConfig.ThreadNetworkKey == NULL)
            {
                printf("Invalid network configuration: Missing Thread network key\n");
                err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
                SuccessOrExit(err);
                ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
            }

            if (netConfig.ThreadNetworkKeyLen == 0)
            {
                printf("Invalid network configuration: Zero-length Thread network key\n");
                err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
                SuccessOrExit(err);
                ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
            }
        }
    }

    else
    {
        printf("Unsupported network type: %d\n", netConfig.NetworkType);
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnsupportedNetworkType);
        SuccessOrExit(err);
        ExitNow(err = WEAVE_ERROR_INVALID_ARGUMENT);
    }

    printf("Network configuration valid\n");

exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleAddNetwork(PacketBuffer *networkInfoTLV)
{
    WEAVE_ERROR  err = WEAVE_NO_ERROR;
    NetworkInfo  newNetworkConfig;
    NetworkInfo *targetNetworkConfig = NULL;
    TLVReader    reader;

    printf("Hande Add network\n");

    reader.Init(networkInfoTLV);

    err = reader.Next();
    SuccessOrExit(err);

    err = newNetworkConfig.Decode(reader);
    SuccessOrExit(err);

    err = ValidateNetworkConfig(newNetworkConfig);
    if (err == WEAVE_ERROR_INVALID_ARGUMENT)
        ExitNow(err = WEAVE_NO_ERROR);
    SuccessOrExit(err);

    if (newNetworkConfig.NetworkType != kNetworkType_Thread)
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnsupportedNetworkType);
        ExitNow();
    }

    for (size_t i = 0; i < kmMaxProvisionSize; i++)
    {
        if (mProvisionNetworks[i].NetworkId == -1)
        {
            targetNetworkConfig = &mProvisionNetworks[i];
            newNetworkConfig.CopyTo(*targetNetworkConfig);
            targetNetworkConfig->NetworkId = i;
        }
    }

    if (targetNetworkConfig == NULL)
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_TooManyNetworks);
        ExitNow();
    }

    printf("Sending AddNetworkComplete response\n");
    printf("  Network Id: %lu\n", (unsigned long)targetNetworkConfig->NetworkId);

    err = SendAddNetworkComplete(targetNetworkConfig->NetworkId);
    SuccessOrExit(err);
exit:
    PacketBuffer::Free(networkInfoTLV);
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleUpdateNetwork(PacketBuffer *networkInfoTLV)
{
    WEAVE_ERROR  err = WEAVE_NO_ERROR;
    NetworkInfo  newNetworkConfig;
    NetworkInfo *targetNetworkConfig = NULL;
    TLVReader    reader;

    reader.Init(networkInfoTLV);

    err = reader.Next();
    SuccessOrExit(err);

    err = newNetworkConfig.Decode(reader);
    SuccessOrExit(err);

    if (newNetworkConfig.NetworkId == -1)
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidNetworkConfiguration);
        ExitNow();
    }

    if (newNetworkConfig.NetworkType != kNetworkType_Thread)
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnsupportedNetworkType);
        ExitNow();
    }

    for (size_t i = 0; i < kmMaxProvisionSize; i++)
    {
        if (mProvisionNetworks[i].NetworkId == newNetworkConfig.NetworkId)
        {
            targetNetworkConfig = &mProvisionNetworks[i];
        }
    }

    if (targetNetworkConfig == NULL)
    {
        printf("Sepcified network not found\n");
        ExitNow();
    }

    {
        NetworkInfo updatedNetworkConfig;

        err = targetNetworkConfig->CopyTo(updatedNetworkConfig);
        SuccessOrExit(err);

        err = newNetworkConfig.MergeTo(updatedNetworkConfig);
        SuccessOrExit(err);

        err = ValidateNetworkConfig(updatedNetworkConfig);
        if (err == WEAVE_ERROR_INVALID_ARGUMENT)
            ExitNow(err = WEAVE_NO_ERROR);
        SuccessOrExit(err);

        err = updatedNetworkConfig.CopyTo(*targetNetworkConfig);
        SuccessOrExit(err);

        err = SendSuccessResponse();
        SuccessOrExit(err);
    }

    SuccessOrExit(err);
exit:
    PacketBuffer::Free(networkInfoTLV);
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleRemoveNetwork(uint32_t networkId)
{
    WEAVE_ERROR  err                   = WEAVE_NO_ERROR;
    NetworkInfo *existingNetworkConfig = NULL;

    for (size_t i = 0; i < kmMaxProvisionSize; i++)
    {
        if (mProvisionNetworks[i].NetworkId == networkId)
        {
            existingNetworkConfig = &mProvisionNetworks[i];
            break;
        }
    }

    if (existingNetworkConfig == NULL)
    {
        printf("Specified network id not found\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnknownNetwork);
        ExitNow();
    }

    existingNetworkConfig->Clear();

    err = SendSuccessResponse();
    SuccessOrExit(err);

exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleGetNetworks(uint8_t flags)
{
    WEAVE_ERROR   err     = WEAVE_NO_ERROR;
    PacketBuffer *respBuf = NULL;
    TLVWriter     writer;
    uint16_t      resultCount = 0;

    respBuf = PacketBuffer::New();
    VerifyOrExit(respBuf != NULL, err = WEAVE_ERROR_NO_MEMORY);

    writer.Init(respBuf);

    err = NetworkInfo::EncodeList(writer, kmMaxProvisionSize, mProvisionNetworks, kNetworkType_Thread, flags,
                                  resultCount);
    SuccessOrExit(err);

    err = writer.Finalize();
    SuccessOrExit(err);

    printf("Sending GetNetworksComplete response\n");
    printf("  Network Count: %d\n", resultCount);

    err     = SendGetNetworksComplete(resultCount, respBuf);
    respBuf = NULL;
    SuccessOrExit(err);

    return WEAVE_NO_ERROR;

exit:
    if (respBuf != NULL)
        PacketBuffer::Free(respBuf);
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleEnableNetwork(uint32_t networkId)
{
    WEAVE_ERROR  err                   = WEAVE_NO_ERROR;
    otError      otErr                 = OT_ERROR_NONE;
    NetworkInfo *existingNetworkConfig = NULL;

    printf("Enable network called\n");
    for (int i = 0; i < kmMaxProvisionSize; i++)
    {
        if (mProvisionNetworks[i].NetworkType != kNetworkType_NotSpecified &&
            mProvisionNetworks[i].NetworkId == networkId)
        {
            existingNetworkConfig = &mProvisionNetworks[i];
            break;
        }
    }

    if (existingNetworkConfig == NULL)
    {
        printf("Specified network id not found\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnknownNetwork);
        ExitNow();
    }

    // only set name, extpanid and masterkey which is configured via add-thread-network
    do
    {
        otExtendedPanId extPanId;
        otMasterKey     key;

        printf("Set name %s\n", existingNetworkConfig->ThreadNetworkName);
        OT_API_CALL(otErr = otThreadSetNetworkName(otrGetInstance(), existingNetworkConfig->ThreadNetworkName));
        printf("1 err %d\n", otErr);
        if (otErr != OT_ERROR_NONE)
        {
            break;
        }

        printf("Set extpanid ");
        for (int i = 0; i < sizeof(extPanId.m8); i++) {
            printf("%02x", existingNetworkConfig->ThreadExtendedPANId[i]);
        }
        printf("\n");
        memcpy(extPanId.m8, existingNetworkConfig->ThreadExtendedPANId, sizeof(extPanId.m8));
        OT_API_CALL(otErr = otThreadSetExtendedPanId(otrGetInstance(), &extPanId));
        printf("3 err %d\n", otErr);
        if (otErr != OT_ERROR_NONE)
        {
            break;
        }

        printf("Set masterkey ");
        for (int i = 0; i < sizeof(key.m8); i++) {
            printf("%02x", existingNetworkConfig->ThreadNetworkKey[i]);
        }
        printf("\n");
        memcpy(key.m8, existingNetworkConfig->ThreadNetworkKey, existingNetworkConfig->ThreadNetworkKeyLen);
        OT_API_CALL(otErr = otThreadSetMasterKey(otrGetInstance(), &key));
        printf("5 err %d\n", otErr);
        if (otErr != OT_ERROR_NONE)
        {
            break;
        }

        OT_API_CALL(otErr = otThreadSetEnabled(otrGetInstance(), true));
        printf("6 err %d\n", otErr);
    } while (0);

    if (otErr != OT_ERROR_NONE)
    {
        printf("ot network configuration failed\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidState);
    }
    else
    {
        err = SendSuccessResponse();
    }

exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleDisableNetwork(uint32_t networkId)
{
    WEAVE_ERROR  err                   = WEAVE_NO_ERROR;
    otError      otErr                 = OT_ERROR_NONE;
    NetworkInfo *existingNetworkConfig = NULL;

    for (int i = 0; i < kmMaxProvisionSize; i++)
    {
        if (mProvisionNetworks[i].NetworkType != kNetworkType_NotSpecified &&
            mProvisionNetworks[i].NetworkId == networkId)
        {
            existingNetworkConfig = &mProvisionNetworks[i];
            break;
        }
    }
    if (existingNetworkConfig == NULL)
    {
        printf("Specified network id not found\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnknownNetwork);
        ExitNow();
    }

    OT_API_CALL(otErr = otThreadSetEnabled(otrGetInstance(), false));

    if (otErr == OT_ERROR_NONE)
    {
        err = SendSuccessResponse();
    }
    else
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_InvalidState);
    }
exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleTestConnectivity(uint32_t networkId)
{
    WEAVE_ERROR  err                   = WEAVE_NO_ERROR;
    NetworkInfo *existingNetworkConfig = NULL;
    otDeviceRole role;

    for (int i = 0; i < kmMaxProvisionSize; i++)
        if (mProvisionNetworks[i].NetworkType != kNetworkType_NotSpecified &&
            mProvisionNetworks[i].NetworkId == networkId)
        {
            existingNetworkConfig = &mProvisionNetworks[i];
            break;
        }

    if (existingNetworkConfig == NULL)
    {
        printf("Specified network id not found\n");
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_UnknownNetwork);
        ExitNow();
    }

    OT_API_CALL(role = otThreadGetDeviceRole(otrGetInstance()));
    if (role != OT_DEVICE_ROLE_DISABLED || OT_DEVICE_ROLE_DETACHED)
    {
        err = SendSuccessResponse();
    }
    else
    {
        err = SendStatusReport(kWeaveProfile_NetworkProvisioning, kStatusCode_NetworkConnectFailed);
    }
    SuccessOrExit(err);

exit:
    return err;
}

WEAVE_ERROR NetworkProvisioningFreeRTOSServer::HandleSetRendezvousMode(uint16_t rendezvousMode)
{
    // Client no need to set
    WEAVE_ERROR err = WEAVE_NO_ERROR;

    {
        char ipAddrStr[64];
        mCurOp->PeerAddr.ToString(ipAddrStr, sizeof(ipAddrStr));
        printf("SetRendezvousMode request received from node %" PRIX64 " (%s)\n", mCurOp->PeerNodeId, ipAddrStr);
        printf("  Rendezvous Mode: %u\n", rendezvousMode);
    }

    err = SendSuccessResponse();
    SuccessOrExit(err);

exit:
    return err;
}

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

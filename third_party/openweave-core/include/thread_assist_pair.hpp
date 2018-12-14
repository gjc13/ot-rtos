#ifndef OTBR_FREERTOS_THREAD_ASSIST_PAIR_H_
#define OTBR_FREERTOS_THREAD_ASSIST_PAIR_H_

#include <FreeRTOS.h>
#include <task.h>

#include "Weave/Core/WeaveError.h"
#include "Weave/Core/WeaveMessageLayer.h"
#include "openthread/thread.h"

#define MAX_SCAN_RESULTS_CNT 8
#define WEAVE_UNSECURE_PORT 11096

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

class ThreadAssistedPairer
{
public:
    ThreadAssistedPairer(TaskHandle_t aTask);
    WEAVE_ERROR StartThreadAssistedPairing();

private:
    WEAVE_ERROR ScanThreadNetworks();
    static void sOtScanHandler(otActiveScanResult *aResult, void *aCtx);
    void        OtScanHandler(otActiveScanResult *aResult);

    static void sWeaveConnectedHandler(WeaveConnection *aConnection, WEAVE_ERROR aError);
    void        WeaveConnectedHandler(WeaveConnection *aConnection, WEAVE_ERROR aError);

    static void sWeaveConnectionCloseHandler(WeaveConnection *aConnection, WEAVE_ERROR aError);
    void        WeaveConectionCloseHandler(WeaveConnection *aConnection, WEAVE_ERROR aError);

    static void sPASEEstablishedHandler(WeaveSecurityManager *sm,
                                        WeaveConnection *     con,
                                        void *                reqState,
                                        uint16_t              sessionKeyId,
                                        uint64_t              peerNodeId,
                                        uint8_t               encType);
    void        PASEEstablishedHandler(WeaveSecurityManager *sm,
                                       WeaveConnection *     con,
                                       void *                reqState,
                                       uint16_t              sessionKeyId,
                                       uint64_t              peerNodeId,
                                       uint8_t               encType);

    static void sPASEEFailedHandler(WeaveSecurityManager *sm,
                                    WeaveConnection *     con,
                                    void *                reqState,
                                    WEAVE_ERROR           localErr,
                                    uint64_t              peerNodeId,
                                    StatusReport *        statusReport);
    void        PASEEFailedHandler(WeaveSecurityManager *sm,
                                   WeaveConnection *     con,
                                   void *                reqState,
                                   WEAVE_ERROR           localErr,
                                   uint64_t              peerNodeId,
                                   StatusReport *        statusReport);

    WEAVE_ERROR StartPair();
    otError     ConnectToPan(otActiveScanResult *aTarget);
    otError     OpenUnsecurePort();
    WEAVE_ERROR ConnectToWeave(otActiveScanResult *aTarget);
    WEAVE_ERROR StartPASE();

    void        SignalPASEError(WEAVE_ERROR err);
    WEAVE_ERROR WaitPASEDone();

    otActiveScanResult mScanResults[MAX_SCAN_RESULTS_CNT];
    int                mNowScanCnt;
    int mNowConnectId;

    WeaveConnection *mWeaveConnection;
    WEAVE_ERROR      mPASEError;

    TaskHandle_t mTask;
};

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

#endif

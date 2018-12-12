#include "apps/misc/time_ntp.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "apps/misc/nat64_utils.h"
#include "openthread/ip6.h"
#include "openthread/openthread-freertos.h"
#include "openthread/sntp.h"

#define NTP_NOTIFY_VALUE (1 << 11)

struct NtpContext
{
    TaskHandle_t mTaskHandle;
    uint64_t     mTime;
    otError      mErr;
};

void ntpHandle(void *aContext, uint64_t aTime, otError aResult)
{
    NtpContext *ctx = static_cast<NtpContext *>(aContext);
    ctx->mErr       = aResult;
    ctx->mTime      = aTime;

    xTaskNotify(ctx->mTaskHandle, NTP_NOTIFY_VALUE, eSetBits);
}

uint64_t timeNtp()
{
    otInstance *  instance = otxGetInstance();
    otSntpQuery   query;
    otMessageInfo messageInfo;
    NtpContext    ctx;
    uint32_t      notifyValue = 0;
    ip6_addr_t    serverAddr;
    if (dnsNat64Address("time.google.com", &serverAddr) == 0)
    {
        memset(&messageInfo, 0, sizeof(messageInfo));
        messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
        memcpy(&messageInfo.mPeerAddr, serverAddr.addr, sizeof(messageInfo.mPeerAddr));
        messageInfo.mPeerPort = OT_SNTP_DEFAULT_SERVER_PORT;

        query.mMessageInfo = &messageInfo;

        ctx.mTaskHandle = xTaskGetCurrentTaskHandle();
        OT_API_CALL(otSntpClientQuery(instance, &query, ntpHandle, &ctx));

        while ((notifyValue & NTP_NOTIFY_VALUE) == 0)
        {
            xTaskNotifyWait(0, NTP_NOTIFY_VALUE, &notifyValue, portMAX_DELAY);
        }

        if (ctx.mErr != OT_ERROR_NONE)
        {
            ctx.mTime = 0;
        }
    }
    else
    {
        ctx.mTime = 0;
    }

    return ctx.mTime;
}

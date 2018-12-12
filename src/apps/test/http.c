#include "user.h"

#include <FreeRTOS.h>
#include <task.h>

#include <openthread/ip6.h>
#include <openthread/openthread-freertos.h>

#include <lwip/sockets.h>

void httpTask(void *p)
{
    struct sockaddr_in6 saddr;
    struct sockaddr_in6 daddr;
    static char         req[]    = "GET / HTTP/1.1\r\nHost: 106.15.231.211:25680\r\n\r\n";
    otInstance *        instance = (otInstance *)p;
    char                res[512];
    int                 fd   = socket(AF_INET6, SOCK_STREAM, 0);
    int                 rval = 0;
    otMessageInfo       messageInfo;

    memset(&daddr, 0, sizeof(daddr));
    daddr.sin6_family = AF_INET6;
    daddr.sin6_port   = htons(25680);
    inet_pton(AF_INET6, "64:ff9b::6a0f:e7d3", &daddr.sin6_addr);
    memset(&messageInfo, 0, sizeof(messageInfo));
    memcpy(&messageInfo.mPeerAddr, &daddr.sin6_addr, sizeof(messageInfo.mPeerAddr));

    OT_API_CALL(otIp6SelectSourceAddress(instance, &messageInfo));
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin6_family = AF_INET6;
    saddr.sin6_port   = htons(25680);
    memcpy(&saddr.sin6_addr, &messageInfo.mSockAddr, sizeof(saddr.sin6_addr));
    if (fd >= 0)
    {
        printf("http client start\r\n");
    }
    else
    {
        printf("http client not start %d\r\n", fd);
        goto exit;
    }
    if (0 == bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)))
    {
        printf("http bind ok\r\n");
    }
    else
    {
        printf("http not bind\r\n");
    }
    rval = connect(fd, (struct sockaddr *)&daddr, sizeof(daddr));
    if (rval == 0)
    {
        printf("http connect ok\r\n");
    }
    else
    {
        printf("http connect failed %d\r\n", rval);
        goto exit;
    }
    rval = send(fd, req, sizeof(req), 0);
    if (rval > 0)
    {
        printf("http send ok\r\n");
    }
    else
    {
        printf("http send failed\r\n");
        goto exit;
    }
    rval      = recv(fd, res, sizeof(res), 0);
    res[rval] = '\0';
    printf("res:%s\r\n", res);

exit:
    printf("http end\r\n");
    if (fd >= 0)
    {
        close(fd);
    }

    gTestTask = NULL;
    vTaskDelete(NULL);
}

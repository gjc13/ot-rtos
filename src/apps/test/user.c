#include "user.h"

#include <string.h>

#include <openthread/cli.h>
#include <openthread/error.h>
#include <openthread/openthread-freertos.h>

TaskHandle_t gTestTask = NULL;

static otError parseLong(char *argv, long *aValue)
{
    char *endptr;
    *aValue = strtol(argv, &endptr, 0);
    return (*endptr == '\0') ? OT_ERROR_NONE : OT_ERROR_PARSE;
}

static void ProcessTest(int argc, char *argv[])
{
    if (argc < 1)
    {
        return;
    }

    if (gTestTask != NULL)
    {
        otCliUartAppendResult(OT_ERROR_BUSY);
    }
    else if (!strcmp(argv[0], "http"))
    {
        xTaskCreate(httpTask, "http", 2048, otxGetInstance(), 2, &gTestTask);
    }
    else if (!strcmp(argv[0], "mqtt"))
    {
        xTaskCreate(mqttTask, "mqtt", 3000, NULL, 2, &gTestTask);
    }
}

static void ProcessEchoServer(int argc, char *argv[])
{
    long port;

    if (argc != 1)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (parseLong(argv[0], &port) != OT_ERROR_NONE)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (!startTcpEchoServer(otxGetInstance(), (uint16_t)port))
    {
        otCliUartAppendResult(OT_ERROR_BUSY);
        return;
    }
}

static void ProcessConnect(int argc, char *argv[])
{
    long port;

    if (argc != 2)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (parseLong(argv[1], &port) != OT_ERROR_NONE)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (!startTcpConnect(otxGetInstance(), argv[0], (uint16_t)port))
    {
        otCliUartAppendResult(OT_ERROR_BUSY);
        return;
    }
}

static void ProcessDisconnect(int argc, char *argv[])
{
    UNUSED_VARIABLE(argc);
    UNUSED_VARIABLE(argv);

    if (!startTcpDisconnect())
    {
        otCliUartAppendResult(OT_ERROR_BUSY);
        return;
    }
}

bool startTcpSend(otInstance *aInstance, uint32_t count, uint32_t size);

static void ProcessSend(int argc, char *argv[])
{
    long count;
    long size;

    if (argc != 2)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (parseLong(argv[1], &count) != OT_ERROR_NONE)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (parseLong(argv[0], &size) != OT_ERROR_NONE)
    {
        otCliUartAppendResult(OT_ERROR_PARSE);
        return;
    }

    if (!startTcpSend(otxGetInstance(), (uint16_t)count, (uint16_t)size))
    {
        otCliUartAppendResult(OT_ERROR_BUSY);
        return;
    }
}

static const struct otCliCommand sCommands[] = {
    {"test", ProcessTest},
    {"tcp_echo_server", ProcessEchoServer},
    {"tcp_connect", ProcessConnect},
    {"tcp_disconnect", ProcessDisconnect},
    {"tcp_send", ProcessSend}
};


void otxUserInit(void)
{
    otCliUartSetUserCommands(sCommands, sizeof(sCommands) / sizeof(sCommands[0]));
}

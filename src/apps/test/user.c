#include "user.h"

#include <string.h>

#include <openthread/cli.h>
#include <openthread/error.h>
#include <openthread/openthread-freertos.h>

TaskHandle_t gTestTask = NULL;

static void ProcessTest(int argc, char *argv[])
{
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

static const struct otCliCommand sCommands[] = {"test", ProcessTest};

void otxUserInit(void)
{
    otCliUartSetUserCommands(sCommands, sizeof(sCommands) / sizeof(sCommands[0]));
}

/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <openthread-core-config.h>
#include <openthread/config.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/sockets.h>

#include <hal/nrf_gpio.h>

#include <openthread/cli.h>
#include <openthread/ip6.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/diag.h>
#include <openthread/platform/radio.h>
#include <openthread/platform/toolchain.h>
#include <openthread/error.h>

#include <utils/code_utils.h>

#include "common/log.h"
#include "platform-nrf5.h"
#include "apps/google_cloud_iot/mqtt_test.h"
#include "openthread/openthread-freertos.h"

typedef enum
{
    kDiagTransmitModeIdle,
    kDiagTransmitModePackets,
    kDiagTransmitModeCarrier
} DiagTrasmitMode;

struct PlatformDiagCommand
{
    const char *mName;
    void (*mCommand)(otInstance *aInstance, int argc, char *argv[], char *aOutput, size_t aOutputMaxLen);
};

struct PlatformDiagMessage
{
    const char mMessageDescriptor[11];
    uint8_t    mChannel;
    int16_t    mID;
    uint32_t   mCnt;
};

/**
 * Diagnostics mode variables.
 *
 */
static bool                       sDiagMode     = false;
static bool                       sListen       = false;
static DiagTrasmitMode            sTransmitMode = kDiagTransmitModeIdle;
static uint8_t                    sChannel      = 20;
static int8_t                     sTxPower      = 0;
static uint32_t                   sTxPeriod     = 1;
static int32_t                    sTxCount      = 0;
static int16_t                    sID           = -1;
static struct PlatformDiagMessage sDiagMessage  = {.mMessageDescriptor = "DiagMessage",
                                                  .mChannel           = 0,
                                                  .mID                = 0,
                                                  .mCnt               = 0};

TaskHandle_t sHttpTask = NULL;
TaskHandle_t sMqttTask = NULL;

void httpTask(void *p)
{
    struct sockaddr_in6 saddr;
    struct sockaddr_in6 daddr;
    static char         req[]    = "GET / HTTP/1.1\r\nHost: 106.15.231.211:25680\r\n\r\n";
    otInstance *        instance = (otInstance *)p;
    char                res[512];

    int fd   = socket(AF_INET6, SOCK_STREAM, 0);
    int rval = 0;

    memset(&daddr, 0, sizeof(daddr));
    daddr.sin6_family = AF_INET6;
    daddr.sin6_port   = htons(25680);
    inet_pton(AF_INET6, "2001:db8:1:ffff::6a0f:e7d3", &daddr.sin6_addr);

    otMessageInfo messageInfo;
    memset(&messageInfo, 0, sizeof(messageInfo));
    memcpy(&messageInfo.mPeerAddr, &daddr.sin6_addr, sizeof(messageInfo.mPeerAddr));

    otxLock();
    otIp6SelectSourceAddress(instance, &messageInfo);
    otxUnlock();

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin6_family = AF_INET6;
    saddr.sin6_port   = htons(25680);
    memcpy(&saddr.sin6_addr, &messageInfo.mSockAddr, sizeof(saddr.sin6_addr));

    if (fd >= 0)
    {
        log("http client start\r\n");
    }
    else
    {
        log("http client not start %d\r\n", fd);
        goto exit;
    }

    if (0 == bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)))
    {
        log("http bind ok\r\n");
    }
    else
    {
        log("http not bind\r\n");
    }

    rval = connect(fd, (struct sockaddr *)&daddr, sizeof(daddr));

    if (rval == 0)
    {
        log("http connect ok\r\n");
    }
    else
    {
        log("http connect failed %d\r\n", rval);
        goto exit;
    }

    rval = send(fd, req, sizeof(req), 0);
    if (rval > 0)
    {
        log("http send ok\r\n");
    }
    else
    {
        log("http send failed\r\n");
        goto exit;
    }

    rval      = recv(fd, res, sizeof(res), 0);
    res[rval] = '\0';
    log("res:%s\r\n", res);

exit:
    log("http end\r\n");
    if (fd >= 0)
    {
        close(fd);
    }

    sHttpTask = NULL;
    vTaskDelete(NULL);
}

static void processHttp(otInstance *aInstance, int argc, char *argv[], char *aOutput, size_t aOutputMaxLen)
{
    if (sHttpTask == NULL)
    {
        UNUSED_VARIABLE(xTaskCreate(httpTask, "http", 2048, aInstance, 2, &sHttpTask));
    }
}

static void processMqtt(otInstance *aInstance, int argc, char *argv[], char *aOutput, size_t aOutputMaxLen)
{
    if (sMqttTask == NULL)
    {
        UNUSED_VARIABLE(xTaskCreate(mqttTask, "mqtt", 3000, aInstance, 2, &sMqttTask));
    }
}

const struct PlatformDiagCommand sCommands[] = {
    {"http", &processHttp},
    {"mqtt", &processMqtt},
};

void otPlatDiagProcess(otInstance *aInstance, int argc, char *argv[], char *aOutput, size_t aOutputMaxLen)
{
    uint32_t i;

    for (i = 0; i < sizeof(sCommands) / sizeof(sCommands[0]); i++)
    {
        if (strcmp(argv[0], sCommands[i].mName) == 0)
        {
            sCommands[i].mCommand(aInstance, argc - 1, argc > 1 ? &argv[1] : NULL, aOutput, aOutputMaxLen);
            break;
        }
    }

    if (i == sizeof(sCommands) / sizeof(sCommands[0]))
    {
        snprintf(aOutput, aOutputMaxLen, "diag feature '%s' is not supported\r\n", argv[0]);
    }
}

void otPlatDiagModeSet(bool aMode)
{
    sDiagMode = aMode;

    if (!sDiagMode)
    {
        otPlatRadioReceive(NULL, sChannel);
        otPlatRadioSleep(NULL);
    }
    else
    {
        // Reinit
        sTransmitMode = kDiagTransmitModeIdle;
    }
}

bool otPlatDiagModeGet()
{
    return sDiagMode;
}

void otPlatDiagChannelSet(uint8_t aChannel)
{
    sChannel = aChannel;
}

void otPlatDiagTxPowerSet(int8_t aTxPower)
{
    sTxPower = aTxPower;
}

void otPlatDiagRadioReceived(otInstance *aInstance, otRadioFrame *aFrame, otError aError)
{
    (void)aInstance;

    if (sListen && (aError == OT_ERROR_NONE))
    {
        if (aFrame->mLength == sizeof(struct PlatformDiagMessage))
        {
            struct PlatformDiagMessage *message = (struct PlatformDiagMessage *)aFrame->mPsdu;

            if (strncmp(message->mMessageDescriptor, "DiagMessage", 11) == 0)
            {
                otPlatLog(OT_LOG_LEVEL_DEBG, OT_LOG_REGION_PLATFORM,
                          "{\"Frame\":{"
                          "\"LocalChannel\":%u ,"
                          "\"RemoteChannel\":%u,"
                          "\"CNT\":%" PRIu32 ","
                          "\"LocalID\":%" PRId16 ","
                          "\"RemoteID\":%" PRId16 ","
                          "\"RSSI\":%d"
                          "}}\r\n",
                          aFrame->mChannel, message->mChannel, message->mCnt, sID, message->mID,
                          aFrame->mInfo.mRxInfo.mRssi);
            }
        }
    }
}

void otPlatDiagAlarmCallback(otInstance *aInstance)
{
    if (sTransmitMode == kDiagTransmitModePackets)
    {
        if ((sTxCount > 0) || (sTxCount == -1))
        {
            otRadioFrame *sTxPacket = otPlatRadioGetTransmitBuffer(aInstance);

            sTxPacket->mLength  = sizeof(struct PlatformDiagMessage);
            sTxPacket->mChannel = sChannel;

            sDiagMessage.mChannel = sTxPacket->mChannel;
            sDiagMessage.mID      = sID;

            memcpy(sTxPacket->mPsdu, &sDiagMessage, sizeof(struct PlatformDiagMessage));
            otPlatRadioTransmit(aInstance, sTxPacket);

            sDiagMessage.mCnt++;

            if (sTxCount != -1)
            {
                sTxCount--;
            }

            uint32_t now = otPlatAlarmMilliGetNow();
            otPlatAlarmMilliStartAt(aInstance, now, sTxPeriod);
        }
        else
        {
            sTransmitMode = kDiagTransmitModeIdle;
            otPlatAlarmMilliStop(aInstance);
            otPlatLog(OT_LOG_LEVEL_DEBG, OT_LOG_REGION_PLATFORM, "Transmit done");
        }
    }
}

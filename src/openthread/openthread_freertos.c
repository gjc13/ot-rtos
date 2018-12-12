/*
 *  Copyright (c) 2018, The OpenThread Authors.
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

#include <openthread/openthread-freertos.h>

#include <assert.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/netdb.h>
#include <lwip/tcpip.h>

#include <openthread/diag.h>
#include <openthread/tasklet.h>

#include "openthread-system.h"

#include "netif.h"
#include "apps/misc/nat64_utils.h"

static TaskHandle_t      sMainTask     = NULL;
static SemaphoreHandle_t sExternalLock = NULL;
static otInstance *      sInstance     = NULL;

static void setupNat64()
{
    ip6_addr_t nat64Prefix;

    nat64Prefix.zone = 0;
    inet_pton(AF_INET6, "64:ff9b::", nat64Prefix.addr);
    setNat64Prefix(&nat64Prefix);
}

static void mainloop(void *aContext)
{
    otInstance *instance = (otInstance *)aContext;

    while (!otSysPseudoResetWasRequested())
    {
        otTaskletsProcess(instance);
        otSysProcessDrivers(instance);
        netifProcess(instance);
        xSemaphoreGive(sExternalLock);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xSemaphoreTake(sExternalLock, portMAX_DELAY);
    }

    otInstanceFinalize(sInstance);
    vTaskDelete(NULL);
}

void otxTaskNotifyGive()
{
    xTaskNotifyGive(sMainTask);
}

void otxTaskNotifyGiveFromISR()
{
    BaseType_t taskWoken;

    vTaskNotifyGiveFromISR(sMainTask, &taskWoken);

    if (taskWoken)
    {
        portYIELD_FROM_ISR(taskWoken);
    }
}

void otTaskletsSignalPending(otInstance *aInstance)
{
    otxTaskNotifyGive();
    // TODO aInstance != sInstance
    // assert(aInstance == sInstance);
}

void otxInit(int argc, char *argv[])
{
    otSysInit(argc, argv);

    sInstance = otInstanceInitSingle();
    assert(sInstance);

#if OPENTHREAD_ENABLE_DIAG
    otDiagInit(sInstance);
#endif
    tcpip_init(netifInit, sInstance);
    setupNat64();

    sExternalLock = xSemaphoreCreateMutex();
    assert(sExternalLock != NULL);
}

void otxStart(void)
{
    xTaskCreate(mainloop, "ot", 4096, sInstance, 2, &sMainTask);
    // Activate deep sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    vTaskStartScheduler();
}

void otxLock(void)
{
    if (xTaskGetCurrentTaskHandle() != sMainTask)
    {
        xSemaphoreTake(sExternalLock, portMAX_DELAY);
    }
}

void otxUnlock(void)
{
    if (xTaskGetCurrentTaskHandle() != sMainTask)
    {
        xSemaphoreGive(sExternalLock);
    }
}

void otSysEventSignalPending(void)
{
    if (otxPortIsInsideInterrupt())
    {
        otxTaskNotifyGiveFromISR();
    }
    else
    {
        otxTaskNotifyGive();
    }
}

otInstance *otxGetInstance()
{
    return sInstance;
}

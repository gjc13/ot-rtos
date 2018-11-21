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

#include "platform-config.h"

#include <openthread-core-config.h>

#include <assert.h>

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/tcpip.h>

#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/tasklet.h>

#include "openthread-system.h"
#include "platform-nrf5.h"

#include "common/default_uart.h"
#include "common/log.h"
#include "apps/misc/nat64_utils.h"
#include "lwip/netdb.h"

static TaskHandle_t      sMainTask     = NULL;
static SemaphoreHandle_t sExternalLock = NULL;
static otInstance *      sInstance     = NULL;

static void setupNat64()
{
    ip6_addr_t nat64Prefix;

    nat64Prefix.zone = 0;
    inet_pton(AF_INET6, "2001:db8:1:ffff::0", nat64Prefix.addr);
    setNat64Prefix(&nat64Prefix);
}

static void mainloop(void *p)
{
    (void)p;

    sExternalLock = xSemaphoreCreateMutex();

    assert(sExternalLock != NULL);
pseudo_reset:

    sInstance = otInstanceInitSingle();
    assert(sInstance);

    otCliUartInit(sInstance);

#if OPENTHREAD_ENABLE_DIAG
    otDiagInit(sInstance);
#endif

    tcpip_init(otxNetifInit, sInstance);
    setupNat64();
    while (!otSysPseudoResetWasRequested())
    {
        otTaskletsProcess(sInstance);
        otSysProcessDrivers(sInstance);
        xSemaphoreGive(sExternalLock);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        xSemaphoreTake(sExternalLock, portMAX_DELAY);
    }

    otInstanceFinalize(sInstance);

    goto pseudo_reset;

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
    (void)aInstance;
}

void otxInit(int argc, char *argv[])
{
    /* Initialize clock driver for better time accuracy in FREERTOS */
    APP_ERROR_CHECK(nrf_drv_clock_init());
    defaultUartInit(NULL);

    otSysInit(argc, argv);
}

void otxStart(void)
{
    UNUSED_VARIABLE(xTaskCreate(mainloop, "ot", 4096, NULL, 2, &sMainTask));
    getDefaultUartIO()->mTaskToWake = sMainTask;

    // Activate deep sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    vTaskStartScheduler();
}

void otxLock(void)
{
    xSemaphoreTake(sExternalLock, portMAX_DELAY);
}

void otxUnlock(void)
{
    xSemaphoreGive(sExternalLock);
}

otInstance *otxGetInstance()
{
    return sInstance;
}

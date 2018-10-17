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

/**
 * @file
 *   This file implements the OpenThread platform abstraction for UART communication.
 *
 */

#include <openthread-core-config.h>
#include <openthread/config.h>

#include <stddef.h>
#include <stdint.h>

#include <utils/code_utils.h>
#include <openthread/platform/toolchain.h>
#include <openthread/platform/uart.h>
#include "openthread/openthread-freertos.h"

#include "openthread-system.h"
#include "platform-nrf5.h"

#include "common/default_uart.h"
#include "common/log.h"

#include <hal/nrf_gpio.h>
#include <hal/nrf_uart.h>
#include <nrf_drv_clock.h>

#if (USB_CDC_AS_SERIAL_TRANSPORT == 0)

bool          sUartEnabled = false;
volatile bool sCanStart    = false;

/**
 *  UART TX buffer variables.
 */
static const uint8_t *sTransmitBuffer = NULL;
static uint16_t       sTransmitLength = 0;

/**
 *  UART RX ring buffer variables.
 */
static uint8_t sReceiveBuffer[UART_RX_BUFFER_SIZE];

/**
 * Function for notifying application about new bytes received.
 */
static void processReceive()
{
    int readSize = uartIORead(getDefaultUartIO(), sReceiveBuffer, UART_RX_BUFFER_SIZE);
    if (readSize > 0)
    {
        otPlatUartReceived(sReceiveBuffer, readSize);
    }
    return;
}

/**
 * Function for notifying application about transmission being done.
 */
static void processTransmit()
{
    otEXPECT(sTransmitBuffer != NULL);

    if (sTransmitBuffer != NULL)
    {
        writeAllToDefaultUart(sTransmitBuffer, sTransmitLength);

        sTransmitBuffer = NULL;
        sTransmitLength = 0;
        otPlatUartSendDone();
    }

exit:
    return;
}

void nrf5UartProcess()
{
    processReceive();
    processTransmit();
}

void nrf5UartInit(void)
{
    // Intentionally empty.
}

void nrf5UartDeinit(void)
{
    if (sUartEnabled)
    {
        otPlatUartDisable();
    }
}

otError otPlatUartEnable(void)
{
    sUartEnabled = true;

    return OT_ERROR_NONE;
}

otError otPlatUartDisable(void)
{
    otError error = OT_ERROR_NONE;

    return error;
}

otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    otError error = OT_ERROR_NONE;

    otEXPECT_ACTION(sTransmitBuffer == NULL, error = OT_ERROR_BUSY);

    sTransmitBuffer = aBuf;
    sTransmitLength = aBufLength;

    otxTaskNotifyGive();
exit:
    return error;
}

#endif // USB_CDC_AS_SERIAL_TRANSPORT == 0

/**
 * The UART driver weak functions definition.
 *
 */
OT_TOOL_WEAK void otPlatUartSendDone(void)
{
}

OT_TOOL_WEAK void otPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength)
{
    (void)aBuf;
    (void)aBufLength;
}

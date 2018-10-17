#include "common/uart_io.h"

static void uartIOHandler(nrf_drv_uart_event_t *aEvent, void *aContext)
{
    UartIO *   uartIO    = (UartIO *)aContext;
    BaseType_t taskWoken = pdFALSE;

    if (aEvent->type == NRF_DRV_UART_EVT_RX_DONE)
    {
        xQueueSendFromISR(uartIO->mRxQueue, aEvent->data.rxtx.p_data, &taskWoken);
        if (uartIO->mTaskToWake)
        {
            vTaskNotifyGiveFromISR(uartIO->mTaskToWake, &taskWoken);
        }
        nrf_drv_uart_rx(uartIO->mUartInstance, &(uartIO->rxBuf), 1);
    }
    else if (aEvent->type == NRF_DRV_UART_EVT_ERROR)
    {
        if (uartIO->mTaskToWake)
        {
            vTaskNotifyGiveFromISR(uartIO->mTaskToWake, &taskWoken);
        }
        nrf_drv_uart_rx(uartIO->mUartInstance, &(uartIO->rxBuf), 1);
    }
    else if (aEvent->type == NRF_DRV_UART_EVT_TX_DONE)
    {
        uartIO->txBytes = aEvent->data.rxtx.bytes;
        xEventGroupSetBitsFromISR(uartIO->mIOEvents, TX_BIT, &taskWoken);
    }
    if (taskWoken)
    {
        portYIELD_FROM_ISR(taskWoken);
    }
}

void uartIOInit(UartIO *aUartIO, UartIOConfig *aConfig)
{
    aUartIO->mRxQueue        = xQueueCreate(aConfig->mBufferDepth, sizeof(uint8_t));
    aUartIO->mReadBlockingIO = aConfig->mReadBlockingIO;
    aUartIO->mIOEvents       = xEventGroupCreate();
    aUartIO->mUartInstance   = aConfig->mUartInstance;
    aUartIO->mFd             = aConfig->mFd;
    aUartIO->mTaskToWake     = aConfig->mTaskToWake;

    aConfig->mUartConfig.p_context = (void *)aUartIO;
    nrf_drv_uart_init(aUartIO->mUartInstance, &(aConfig->mUartConfig), uartIOHandler);

    nrf_drv_uart_rx(aUartIO->mUartInstance, &(aUartIO->rxBuf), 1);
}

int uartIORead(UartIO *aUartIO, void *aBuf, size_t aLength)
{
    int ret = 0;

    if (aBuf == NULL || aUartIO == NULL)
    {
        ret = -1;
    }
    else if (aLength > 0)
    {
        uint8_t *  buf  = (uint8_t *)aBuf;
        TickType_t wait = aUartIO->mReadBlockingIO ? portMAX_DELAY : 0;
        // First block till one byte arrives
        if (xQueueReceive(aUartIO->mRxQueue, &buf[ret], wait))
        {
            ret++;
            while (ret < aLength && xQueueReceive(aUartIO->mRxQueue, &buf[ret], 0))
            {
                ret++;
            }
        }
    }
    return ret;
}

int uartIOWrite(UartIO *aUartIO, const void *aBuf, size_t aLength)
{
    int ret = 0;

    if (aUartIO == NULL || aBuf == NULL)
    {
        ret = -1;
    }

    if (aLength > 0)
    {
        uint8_t writeSize = aLength < UINT8_MAX ? aLength : UINT8_MAX;
        xEventGroupClearBits(aUartIO->mIOEvents, TX_BIT);
        nrf_drv_uart_tx(aUartIO->mUartInstance, aBuf, writeSize);
        xEventGroupWaitBits(aUartIO->mIOEvents, TX_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
        ret = aUartIO->txBytes;
    }

    return ret;
}

int readAll(UartIO *aUartIO, void *aBuf, size_t aSize)
{
    int      ret = 0;
    uint8_t *buf = (uint8_t *)aBuf;
    while ((size_t)ret < aSize)
    {
        ssize_t thisRet = uartIORead(aUartIO, buf + ret, aSize - (size_t)ret);
        if (ret < 0)
        {
            ret = thisRet;
            break;
        }
        else
        {
            ret += thisRet;
        }
    }
    return ret;
}

int writeAll(UartIO *aUartIO, const void *aBuf, size_t aSize)
{
    int            ret = 0;
    const uint8_t *buf = (const uint8_t *)aBuf;
    while ((size_t)ret < aSize)
    {
        ssize_t thisRet = uartIOWrite(aUartIO, buf + ret, aSize - (size_t)ret);
        if (ret < 0)
        {
            ret = thisRet;
            break;
        }
        else
        {
            ret += thisRet;
        }
    }
    return ret;
}

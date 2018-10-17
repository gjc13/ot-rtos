#ifndef UART_IO_H_
#define UART_IO_H_

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <queue.h>
#include <semphr.h>
#include <nrfx_uart.h>
#include <nrf_drv_uart.h>
#include <app_fifo.h>

#define UART_PIN_DISCONNECTED 0xFFFFFFFF
#define TX_BIT (1 << 0)
#define RX_BIT (1 << 17)
#define ERR_BIT (1 << 18)


#define IS_RX_READY(aState, aUartIO) (aState | (1 << (aUartIO->mFd)) | RX_BIT)

typedef struct
{
    nrf_drv_uart_t *mUartInstance;

    TaskHandle_t mTaskToWake;
    EventGroupHandle_t mIOEvents;

    QueueHandle_t mRxQueue;
    uint8_t rxBuf;

    int txBytes;

    bool mReadBlockingIO;

    uint8_t mFd;
} UartIO;

typedef struct
{
    nrf_drv_uart_t *mUartInstance;
    nrf_drv_uart_config_t mUartConfig;
    TaskHandle_t mTaskToWake;
    bool mReadBlockingIO;

    size_t mBufferDepth;

    uint8_t mFd;
} UartIOConfig;

void uartIOInit(UartIO *aUartIO, UartIOConfig *aConfig);

int uartIORead(UartIO *aUartIO, void *aBuf, size_t aLength);

int uartIOWrite(UartIO *aUartIO, const void *aBuf, size_t aLength);

int readAll(UartIO *aUartIO, void *aBuf, size_t aSize);

int writeAll(UartIO *aUartIO, const void *aBuf, size_t aSize);

#endif // UART_IO_H_


#include "common/default_uart.h"

#include <errno.h>
#include <stdio.h>
#include <bsp.h>

#define STDOUT_FILENO 1
#define STDERR_FILENO 2

static xSemaphoreHandle sDefaultUartIOMtx;
static nrf_drv_uart_t   sDefaultUartInst = NRF_DRV_UART_INSTANCE(0);
static UartIO           sDefaultUartIO;

void defaultUartInit(TaskHandle_t aTaskToWake)
{
    UartIOConfig          cfg;
    nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;

    config.baudrate           = UART_BAUDRATE_BAUDRATE_Baud115200;
    config.hwfc               = NRF_UART_HWFC_DISABLED;
    config.interrupt_priority = 6;
    config.parity             = NRF_UART_PARITY_EXCLUDED;
    config.pselcts            = CTS_PIN_NUMBER;
    config.pselrts            = RTS_PIN_NUMBER;
    config.pselrxd            = RX_PIN_NUMBER;
    config.pseltxd            = TX_PIN_NUMBER;

    cfg.mUartConfig     = config;
    cfg.mUartInstance   = &sDefaultUartInst;
    cfg.mReadBlockingIO = false;
    cfg.mBufferDepth    = 256;
    cfg.mFd             = 0;
    cfg.mTaskToWake     = aTaskToWake;

    uartIOInit(&sDefaultUartIO, &cfg);

    sDefaultUartIOMtx = xSemaphoreCreateMutex();

    // initialize stderr
    // This is a walkaround for libc setting stderr without __SWR
    fprintf(stdout, "");
    stderr->_flags = __SORD | __SWR | __SNBF;
}

int writeToDefaultUart(const void *aBuf, size_t aSize)
{
    int ret;

    xSemaphoreTake(sDefaultUartIOMtx, portMAX_DELAY);
    ret = uartIOWrite(&sDefaultUartIO, aBuf, aSize);
    xSemaphoreGive(sDefaultUartIOMtx);

    return ret;
}

int writeAllToDefaultUart(const void *aBuf, size_t aSize)
{
    int ret;

    xSemaphoreTake(sDefaultUartIOMtx, portMAX_DELAY);
    ret = writeAll(&sDefaultUartIO, aBuf, aSize);
    xSemaphoreGive(sDefaultUartIOMtx);

    return ret;
}

UartIO *getDefaultUartIO()
{
    return &sDefaultUartIO;
}

int _write(int file, const char *p_char, int len)
{
    int ret = 0;

    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        ret = writeToDefaultUart(p_char, len);
    }
    else
    {
        errno = EBADF;
        ret   = -1;
    }

    return ret;
}

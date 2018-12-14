#include "openthread/uart_lock.h"

#include <FreeRTOS.h>
#include <semphr.h>

#if OPENTHREAD_UART_LOCK

/**
 * UART Lock
 */
static xSemaphoreHandle sUartMtx;

otError otrUartLockInit()
{
    sUartMtx = xSemaphoreCreateMutex();
    return OT_ERROR_NONE;
}

otError otCliUartLock()
{
    xSemaphoreTake(sUartMtx, portMAX_DELAY);
    return OT_ERROR_NONE;
}

otError otCliUartUnlock()
{
    xSemaphoreGive(sUartMtx);
    return OT_ERROR_NONE;
}

#endif // OPENTHREAD_UART_LOCK

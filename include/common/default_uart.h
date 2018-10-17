#ifndef DEFAULT_UART_H_
#define DEFAULT_UART_H_

#include "uart_io.h"

void defaultUartInit(TaskHandle_t aTaskToWake);

//Default uart is used by multiple thread as log output so need extra lock mechanism
int writeToDefaultUart(const void *aBuf, size_t aSize);

int writeAllToDefaultUart(const void *aBuf, size_t aSize);

UartIO *getDefaultUartIO();

#endif

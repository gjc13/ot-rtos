#ifndef OT_FREERTOS_PORTABLE_H_
#define OT_FREERTOS_PORTABLE_H_

#if defined PLATFORM_nrf52

#include "core_cm4.h"
#include "nordic_common.h"

#define PORT_ENABLE_SLEEP()                \
    do                                     \
    {                                      \
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; \
    } while (0)

#define PORT_GET_IN_ISR(x)                                  \
    do                                                      \
    {                                                       \
        __asm volatile("mrs %0, ipsr" : "=r"(x)::"memory"); \
    } while (0)

#else

#define PORT_ENABLE_SLEEP() \
    do                      \
    {                       \
    } while (0)

#define PORT_GET_IN_ISR(x) \
    do                     \
    {                      \
        x = 0;             \
    } while (0)

#define UNUSED_VARIABLE(x) ((void)(x))

#endif

#endif

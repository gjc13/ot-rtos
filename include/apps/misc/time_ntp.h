#ifndef OT_RTOS_TIME_NTP_H_
#define OT_RTOS_TIME_NTP_H_

#include "stdint.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns current time from ntp server configed in openthread
 *
 */
uint64_t timeNtp();

#ifdef __cplusplus
}
#endif

#endif

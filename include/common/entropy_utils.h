#ifndef OT_RTOS_ENTROPY_UTILS_H_
#define OT_RTOS_ENTROPY_UTILS_H_

#include <mbedtls/entropy.h>
#include <mbedtls/entropy_poll.h>

#ifdef __cplusplus
extern "C" {
#endif

int HandleMbedtlsEntropyPoll(void *aData, unsigned char *aOutput, size_t aInLen, size_t *aOutLen);

#ifdef __cplusplus
}
#endif

#endif

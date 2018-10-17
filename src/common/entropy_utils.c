#include "common/entropy_utils.h"
#include "openthread/platform/random.h"

int HandleMbedtlsEntropyPoll(void *aData, unsigned char *aOutput, size_t aInLen, size_t *aOutLen)
{
    (void)aData;

    int     rval  = 0;
    otError error = otPlatRandomGetTrue((uint8_t *)aOutput, (uint16_t)aInLen);

    if (aOutLen != NULL)
    {
        *aOutLen = aInLen;
    }

    if (error != OT_ERROR_NONE)
    {
        rval = MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
    }
    return rval;
}

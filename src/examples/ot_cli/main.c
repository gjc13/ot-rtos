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

#include <openthread-core-config.h>
#include <openthread/openthread-freertos.h>
#include <openthread-system.h>

#include <errno.h>
#include <openthread/cli.h>
#include <openthread/platform/uart.h>

#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#if OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_APP
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    OT_UNUSED_VARIABLE(aLogLevel);
    OT_UNUSED_VARIABLE(aLogRegion);
    OT_UNUSED_VARIABLE(aFormat);

    va_list ap;
    va_start(ap, aFormat);
    otCliPlatLogv(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif

int _write(int file, const char *p_char, int len)
{
    int ret = len;

    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        otCliUartOutput(p_char, len);
    }
    else
    {
        errno = EBADF;
        ret   = -1;
    }

    return ret;
}

#if PLATFORM_linux || PLATFORM_linux_radio
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

jmp_buf gResetJump;

void vApplicationMallocFailedHook(void)
{
    printf("failed malloc\n");
    exit(-1);
}

void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
    UNUSED_VARIABLE(pcFileName);
    UNUSED_VARIABLE(ulLine);
}

#endif

int main(int argc, char *argv[])
{
#if PLATFORM_linux
    if (setjmp(gResetJump))
    {
        alarm(0);
        execvp(argv[0], argv);
    }
#endif
    otrInit(argc, argv);
    otCliUartInit(otrGetInstance());
    otrUserInit();
    otrStart();
    return 0;
}

/*
 *  Copyright (c) 2019, The OpenThread Authors.
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

#include "otr_system.h"

#if PLATFORM_linux

#include <errno.h>
#include <unistd.h>

#include <platform-posix.h>
#include <openthread/tasklet.h>

static struct otrSystemCtx
{
    fd_set read_fds;
    fd_set write_fds;
    fd_set error_fds;
} sCtx;

void otrSystemPoll(otInstance *aInstance)
{
    int            max_fd = -1;
    struct timeval timeout;
    int            rval;

    FD_ZERO(&sCtx.read_fds);
    FD_ZERO(&sCtx.write_fds);
    FD_ZERO(&sCtx.error_fds);

    platformUartUpdateFdSet(&sCtx.read_fds, &sCtx.write_fds, &sCtx.error_fds, &max_fd);
    platformRadioUpdateFdSet(&sCtx.read_fds, &sCtx.write_fds, &max_fd);
    platformAlarmUpdateTimeout(&timeout);

    if (!otTaskletsArePending(aInstance))
    {
        rval = select(max_fd + 1, &sCtx.read_fds, &sCtx.write_fds, &sCtx.error_fds, &timeout);

        if ((rval < 0) && (errno != EINTR))
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
    }
}

void otrSystemProcess(otInstance *aInstance)
{
    platformUartProcess();
    platformRadioProcess(aInstance, &sCtx.read_fds, &sCtx.write_fds);
    platformAlarmProcess(aInstance);
}

#else

#include <FreeRTOS.h>
#include <task.h>

#include <openthread-system.h>
#include <openthread/tasklet.h>

void otrSystemPoll(otInstance *aInstance)
{
    if (!otTaskletsArePending(aInstance))
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

void otrSystemProcess(otInstance *aInstance)
{
    otSysProcessDrivers(aInstance);
}

#endif // PLATFORM_linux

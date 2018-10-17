/*
 *  Copyright (c) 2018, The OpenThread Authors.
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

/**
 * @file
 * @brief
 *   This file includes the platform UDP driver.
 */

#include "esp32_platform.h"

#include <arpa/inet.h>
#include <lwip/ip.h>
#include <lwip/ip6.h>
#include <lwip/tcpip.h>
#include <lwip/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openthread/udp.h>
#include <openthread/platform/udp.h>

#include "common/code_utils.hpp"

#include "event_queue_api.h"

#if OPENTHREAD_ENABLE_PLATFORM_UDP

static const size_t kMaxUdpSize = 1280;

struct ReceiveEvent
{
    otUdpSocket * mUdpSocket;
    otMessageInfo mMessageInfo;
    uint16_t      mLength;
    uint8_t       mPayload[1];
};

static bool IsIp4(const otIp6Address &aAddress)
{
    static const uint16_t kIp4Prefix[] = {0, 0, 0, 0, 0, 0xffff};
    return 0 == memcmp(aAddress.mFields.m16, kIp4Prefix, sizeof(kIp4Prefix));
}

static void udpReceive(void *           aContext,
                       struct udp_pcb * aUdpPcb,
                       struct pbuf *    aBuffer,
                       const ip_addr_t *aAddress,
                       u16_t            aPort)
{
    err_t         err       = ERR_OK;
    otUdpSocket * udpSocket = static_cast<otUdpSocket *>(aContext);
    ReceiveEvent *event     = NULL;

    VerifyOrExit(aBuffer != NULL);
    event = static_cast<ReceiveEvent *>(malloc(sizeof(*event) + aBuffer->tot_len));
    VerifyOrExit(event != NULL, err = ERR_BUF);

    event->mLength = aBuffer->tot_len;
    if (aAddress->type == IPADDR_TYPE_V4)
    {
        event->mMessageInfo.mHopLimit = IPH_TTL(ip4_current_header());
        ip4_2_ipv4_mapped_ipv6(reinterpret_cast<ip6_addr_t *>(&event->mMessageInfo.mSockAddr), ip4_current_dest_addr());
        ip4_2_ipv4_mapped_ipv6(reinterpret_cast<ip6_addr_t *>(&event->mMessageInfo.mPeerAddr), ip_2_ip4(aAddress));
    }
    else
    {
        event->mMessageInfo.mHopLimit = IP6H_HOPLIM(ip6_current_header());
        memcpy(&event->mMessageInfo.mPeerAddr, ip_2_ip6(aAddress), sizeof(event->mMessageInfo.mPeerAddr));
        memcpy(&event->mMessageInfo.mSockAddr, ip6_current_dest_addr(), sizeof(event->mMessageInfo.mSockAddr));
    }

    event->mMessageInfo.mSockPort = udpSocket->mSockName.mPort;
    event->mMessageInfo.mPeerPort = aPort;
    event->mMessageInfo.mInterfaceId =
        (ip_current_input_netif() == otxGetNetif() ? OT_NETIF_INTERFACE_ID_THREAD : OT_NETIF_INTERFACE_ID_HOST);
    event->mUdpSocket = udpSocket;

    VerifyOrExit(aBuffer->tot_len == pbuf_copy_partial(aBuffer, event->mPayload, aBuffer->tot_len, 0), err = ERR_ARG);

    // TODO change API or save instance
    VerifyOrExit(OT_ERROR_NONE == otEventQueuePush(NULL, EVENT_UDP_RECEIVE, event, 0), err = ERR_IF);

exit:
    if (err != ERR_OK)
    {
        if (event != NULL)
        {
            free(event);
        }
    }

    if (aBuffer != NULL)
    {
        pbuf_free(aBuffer);
    }
}

otError otPlatUdpSocket(otUdpSocket *aUdpSocket)
{
    assert(aUdpSocket->mHandle == NULL);
    return OT_ERROR_NONE;
}

otError otPlatUdpClose(otUdpSocket *aUdpSocket)
{
    struct udp_pcb *up = reinterpret_cast<struct udp_pcb *>(aUdpSocket->mHandle);

    LOCK_TCPIP_CORE();
    VerifyOrExit(up != NULL);

    udp_remove(up);
    aUdpSocket->mHandle = NULL;
exit:
    UNLOCK_TCPIP_CORE();
    return OT_ERROR_NONE;
}

otError otPlatUdpBind(otUdpSocket *aUdpSocket)
{
    otError         error = OT_ERROR_NONE;
    struct udp_pcb *up    = reinterpret_cast<struct udp_pcb *>(aUdpSocket->mHandle);
    ip_addr_t       ipaddr;

    assert(up == NULL);
    VerifyOrExit(up == NULL, error = OT_ERROR_ALREADY);
    LOCK_TCPIP_CORE();

    up = udp_new_ip_type(IPADDR_TYPE_ANY);

    ipaddr.type = IPADDR_TYPE_ANY;
    memcpy(ip_2_ip6(&ipaddr), &aUdpSocket->mSockName.mAddress, sizeof(ip6_addr_t));
    VerifyOrExit(ERR_OK == udp_bind(up, &ipaddr, aUdpSocket->mSockName.mPort), error = OT_ERROR_FAILED);

    if (aUdpSocket->mSockName.mPort == 0)
    {
        aUdpSocket->mSockName.mPort = up->local_port;
    }

    udp_recv(up, udpReceive, aUdpSocket);
    aUdpSocket->mHandle = up;

exit:
    UNLOCK_TCPIP_CORE();
    return error;
}

otError otPlatUdpConnect(otUdpSocket *aUdpSocket)
{
    otError         error = OT_ERROR_NONE;
    struct udp_pcb *up    = reinterpret_cast<struct udp_pcb *>(aUdpSocket->mHandle);
    ip_addr_t       ipaddr;

    if (IsIp4(aUdpSocket->mPeerName.mAddress))
    {
        ipaddr.type = IPADDR_TYPE_V4;
        unmap_ipv4_mapped_ipv6(ip_2_ip4(&ipaddr),
                               reinterpret_cast<const ip6_addr_t *>(&aUdpSocket->mPeerName.mAddress));
    }
    else
    {
        ipaddr.type = IPADDR_TYPE_V6;
        memcpy(ip_2_ip6(&ipaddr), &aUdpSocket->mPeerName.mAddress, sizeof(ip6_addr_t));
    }
    LOCK_TCPIP_CORE();
    VerifyOrExit(udp_connect(up, &ipaddr, aUdpSocket->mPeerName.mPort) == ERR_OK, error = OT_ERROR_FAILED);

    if (aUdpSocket->mSockName.mPort == 0)
    {
        aUdpSocket->mSockName.mPort = up->local_port;
    }

exit:
    UNLOCK_TCPIP_CORE();
    return error;
}

static void lwipAddrFromIp6Address(ip_addr_t &aTo, const otIp6Address &aFrom)
{
    if (IsIp4(aFrom))
    {
        aTo.type = IPADDR_TYPE_V4;
        unmap_ipv4_mapped_ipv6(ip_2_ip4(&aTo), reinterpret_cast<const ip6_addr_t *>(&aFrom));
    }
    else
    {
        aTo.type = IPADDR_TYPE_V6;
        memcpy(ip_2_ip6(&aTo), &aFrom, sizeof(ip6_addr_t));
    }
}

otError otPlatUdpSend(otUdpSocket *aUdpSocket, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otError         error  = OT_ERROR_NONE;
    struct udp_pcb *up     = reinterpret_cast<struct udp_pcb *>(aUdpSocket->mHandle);
    struct pbuf *   buffer = NULL;
    uint16_t        length = otMessageGetLength(aMessage);
    ip_addr_t       dstaddr;
    ip_addr_t       srcaddr;

    lwipAddrFromIp6Address(dstaddr, aMessageInfo->mPeerAddr);
    lwipAddrFromIp6Address(srcaddr, aMessageInfo->mSockAddr);
    srcaddr.type = dstaddr.type;

    LOCK_TCPIP_CORE();
    buffer = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_RAM);
    VerifyOrExit(otMessageRead(aMessage, 0, buffer->payload, length) == length, error = OT_ERROR_INVALID_ARGS);
    if (ip_addr_isany(&srcaddr))
    {
        VerifyOrExit(ERR_OK == udp_sendto(up, buffer, &dstaddr, aMessageInfo->mPeerPort), error = OT_ERROR_FAILED);
    }
    else
    {
        struct netif *netif = ip_route(&srcaddr, &dstaddr);
        VerifyOrExit(ERR_OK == udp_sendto_if_src(up, buffer, &dstaddr, aMessageInfo->mPeerPort, netif, &srcaddr),
                     error = OT_ERROR_FAILED);
    }

exit:
    UNLOCK_TCPIP_CORE();
    if (error == OT_ERROR_NONE)
    {
        otMessageFree(aMessage);
    }

    if (buffer != NULL)
    {
        pbuf_free(buffer);
    }
    return error;
}

void platformUdpInit(int aNetifIndex)
{
}

void esp32UdpProcess(otInstance *aInstance, int aType, const void *aData)
{
    otMessage *         message = NULL;
    const ReceiveEvent *event   = NULL;

    VerifyOrExit(aType == EVENT_UDP_RECEIVE);

    event = static_cast<const ReceiveEvent *>(aData);

    VerifyOrExit((message = otUdpNewMessage(aInstance, false)) != NULL);
    VerifyOrExit(otMessageAppend(message, event->mPayload, event->mLength) == OT_ERROR_NONE);

    event->mUdpSocket->mHandler(event->mUdpSocket->mContext, message, &event->mMessageInfo);
    otMessageFree(message);

exit:
    return;
}
#endif // OPENTHREAD_ENABLE_PLATFORM_UDP

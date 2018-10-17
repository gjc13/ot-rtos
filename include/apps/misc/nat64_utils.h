#ifndef OT_RTOS_NAT64_UTILS_H_
#define OT_RTOS_NAT64_UTILS_H_

#include "lwip/ip6.h"
#include "lwip/ip4.h"

#ifdef __cplusplus
extern "C" {
#endif

void setNat64Prefix(const ip6_addr_t* aNat64Prefix);

ip6_addr_t getNat64Address(const ip4_addr_t* aIpv4Address);

int dnsNat64Address(const char* aHostName, ip6_addr_t* aAddrOut);


#ifdef __cplusplus
}
#endif

#endif

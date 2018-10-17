#include "apps/misc/nat64_utils.h"
#include "common/log.h"

#include "lwip/netdb.h"

static ip6_addr_t sNat64Prefix;

void setNat64Prefix(const ip6_addr_t *aNat64Prefix)
{
    sNat64Prefix = *aNat64Prefix;
}

ip6_addr_t getNat64Address(const ip4_addr_t *aIpv4Address)
{
    ip6_addr_t addr = sNat64Prefix;
    addr.addr[3]    = aIpv4Address->addr;
    addr.zone       = IP6_NO_ZONE;
    return addr;
}

int dnsNat64Address(const char *aHostName, ip6_addr_t *aAddrOut)
{
    ip4_addr_t      v4Addr;
    struct hostent *host = gethostbyname(aHostName);
    int             ret  = -1;
    if (host && host->h_addr_list && host->h_addrtype == AF_INET)
    {
        memcpy(&v4Addr.addr, host->h_addr_list[0], sizeof(v4Addr.addr));
        *aAddrOut = getNat64Address(&v4Addr);
        ret       = 0;
    }

    // log("Dns done\r\n");
    return ret;
}

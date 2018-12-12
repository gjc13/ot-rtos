#ifndef OTX_NETIF_H
#define OTX_NETIF_H

#ifdef __cplusplus
extern "C" {
#endif

void netifInit(void *aContext);
void netifProcess(otInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // OTX_NETIF_H

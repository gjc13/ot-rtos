#ifndef OTBR_FREERTOS_OPENWEAVE_NETWORK_PROVISIONING_H_
#define OTBR_FREERTOS_OPENWEAVE_NETWORK_PROVISIONING_H_

#include "Weave/Profiles/network-provisioning/NetworkProvisioning.h"
#include "Weave/Profiles/network-provisioning/NetworkInfo.h"

#include "openthread/thread.h"

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

class NetworkProvisioningFreeRTOSServer : public Profiles::NetworkProvisioning::NetworkProvisioningDelegate,
                                          public Profiles::NetworkProvisioning::NetworkProvisioningServer
{
public:
    WEAVE_ERROR Init(WeaveExchangeManager *exchangeMgr);

    // ===== Members that override virtual methods on NetworkProvisioningDelegate
    WEAVE_ERROR HandleScanNetworks(uint8_t networkType) override;
    WEAVE_ERROR HandleAddNetwork(PacketBuffer *networkInfoTLV) override;
    WEAVE_ERROR HandleUpdateNetwork(PacketBuffer *networkInfoTLV) override;
    WEAVE_ERROR HandleRemoveNetwork(uint32_t networkId) override;
    WEAVE_ERROR HandleGetNetworks(uint8_t flags) override;
    WEAVE_ERROR HandleEnableNetwork(uint32_t networkId) override;
    WEAVE_ERROR HandleDisableNetwork(uint32_t networkId) override;
    WEAVE_ERROR HandleTestConnectivity(uint32_t networkId) override;
    WEAVE_ERROR HandleSetRendezvousMode(uint16_t rendezvousMode) override;

private:
    WEAVE_ERROR ValidateNetworkConfig(Profiles::NetworkProvisioning::NetworkInfo& netConfig);

    WEAVE_ERROR ScanThreadNetworks();
    static void sOtScanHandler(otActiveScanResult *result, void *ctx);
    void otScanHandler(otActiveScanResult *result);
    WEAVE_ERROR SendScannedNetworks();

    Profiles::NetworkProvisioning::NetworkInfo* mScannedNetworks;
    uint16_t mScannedNetworksSize;
    uint16_t mScannedNetworksCapacity;
    static const size_t kmDefaultCapacity = 8;

    static const size_t kmMaxProvisionSize = 4;
    Profiles::NetworkProvisioning::NetworkInfo mProvisionNetworks[kmMaxProvisionSize];
};

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

#endif

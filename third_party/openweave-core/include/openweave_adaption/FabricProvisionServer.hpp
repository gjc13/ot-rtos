#include "Weave/Profiles/WeaveProfiles.h"
#include "Weave/Profiles/fabric-provisioning/FabricProvisioning.h"

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

class FabricProvisioningFreeRTOSServer : public Profiles::FabricProvisioning::FabricProvisioningDelegate,
                                         public Profiles::FabricProvisioning::FabricProvisioningServer
{
public:
    WEAVE_ERROR Init(WeaveExchangeManager *exchangeMgr);
    WEAVE_ERROR HandleCreateFabric(void) override;
    WEAVE_ERROR HandleJoinExistingFabric(void) override;
    WEAVE_ERROR HandleLeaveFabric(void) override;
    WEAVE_ERROR HandleGetFabricConfig(void) override;
private:
};

} // namespace OtFreeRTOS
} // namespace Weave
} // namespace nl

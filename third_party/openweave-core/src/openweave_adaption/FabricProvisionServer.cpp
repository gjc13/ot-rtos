#include "openweave_adaption/FabricProvisionServer.hpp"

using namespace nl::Weave::Profiles::FabricProvisioning;

namespace nl {
namespace Weave {
namespace OtFreeRTOS {

WEAVE_ERROR FabricProvisioningFreeRTOSServer::Init(WeaveExchangeManager *exchangeMgr)
{
    WEAVE_ERROR err;

    // Initialize the base class.
    err = this->FabricProvisioningServer::Init(exchangeMgr);
    SuccessOrExit(err);

    // Tell the base class that it should delegate service provisioning requests to us.
    SetDelegate(this);

exit:
    return err;
}

WEAVE_ERROR FabricProvisioningFreeRTOSServer::HandleCreateFabric() {
    printf("Weave fabric created (fabric id %llX)\n", (unsigned long long)FabricState->FabricId);
    return SendSuccessResponse();
}

WEAVE_ERROR FabricProvisioningFreeRTOSServer::HandleJoinExistingFabric() {
    printf("Joined existing Weave fabric (fabric id %llX)\n", (unsigned long long)FabricState->FabricId);
    return SendSuccessResponse();
}

WEAVE_ERROR FabricProvisioningFreeRTOSServer::HandleLeaveFabric() {
    printf("LeaveFabric complete\n");
    return SendSuccessResponse();
}

WEAVE_ERROR FabricProvisioningFreeRTOSServer::HandleGetFabricConfig() {
    printf("GetFabricConfig complete\n");
    return WEAVE_NO_ERROR;
}

}
}
}

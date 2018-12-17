#include "user.h"

#include "FreeRTOS.h"
#include "task.h"

#include "thread_assist_pair.hpp"
#include "Weave/Core/WeaveGlobals.h"
#include "Weave/Core/WeaveMessageLayer.h"
#include "openweave_adaption/entropy.hpp"
#include "openweave_adaption/NetworkProvisionServer.hpp"
#include "test-apps/MockDDServer.h"

using namespace nl::Weave;
using namespace nl::Weave::OtFreeRTOS;

void weavePairTask(void *p)
{
    WEAVE_ERROR                                 err;
    sys_mbox_t                                  system_mbox;
    nl::Weave::System::Layer                    SystemLayer;
    nl::Inet::InetLayer                         Inet;
    WeaveMessageLayer::InitContext              initContext;
    TaskHandle_t *                              task = reinterpret_cast<TaskHandle_t *>(p);
    nl::Weave::OtFreeRTOS::ThreadAssistedPairer pairer(*task);
    MockDeviceDescriptionServer                 ddServer;
    NetworkProvisioningFreeRTOSServer           npServer;

    printf("Start openweave init\n");

    // Initialize the layers of the Weave stack.
    sys_mbox_new(&system_mbox, 20);

    printf("Init system layer\n");
    err = SystemLayer.Init(system_mbox);
    if (err != WEAVE_NO_ERROR)
    {
        printf("SystemLayer.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Init entropy");
    err = nl::Weave::OtFreeRTOS::InitEntropy();
    if (err != WEAVE_NO_ERROR)
    {
        printf("InitEntropy failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Init inet\n");
    err = Inet.Init(SystemLayer, system_mbox);
    if (err != WEAVE_NO_ERROR)
    {
        printf("Inet.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Init fabric state");
    err                     = FabricState.Init();
    FabricState.PairingCode = "UWR4K5";
    // XXX: for test only
    FabricState.LocalNodeId = 0x18B43000002DCF71ULL;
    if (err != WEAVE_NO_ERROR)
    {
        printf("FabricState.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    initContext.systemLayer = &SystemLayer;
    initContext.inet        = &Inet;
    initContext.fabricState = &FabricState;
    initContext.listenTCP   = true;
    initContext.listenUDP   = false;

    printf("init message layer\n");
    err = MessageLayer.Init(&initContext);
    if (err != WEAVE_NO_ERROR)
    {
        printf("MessageLayer.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("init exchange mgr\n");
    err = ExchangeMgr.Init(&MessageLayer);
    if (err != WEAVE_NO_ERROR)
    {
        printf("WeaveExchangeManager.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("init security mgr\n");
    err = SecurityMgr.Init(ExchangeMgr, SystemLayer);
    if (err != WEAVE_NO_ERROR)
    {
        printf("SecurityMgr.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Init device description server\n");
    err = ddServer.Init(&ExchangeMgr);
    if (err != WEAVE_NO_ERROR)
    {
        printf("ddServer.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Init network provisioning server\n");
    err = npServer.Init(&ExchangeMgr);
    if (err != WEAVE_NO_ERROR)
    {
        printf("npServer.Init failed: %s\n", nl::ErrorStr(err));
        exit(EXIT_FAILURE);
    }

    printf("Openweave init done\n");

    pairer.StartThreadAssistedPairing();
    // Initialize the EchoClient EchoSever application.
    while (true)
    {
        SystemLayer.DispatchEvents();
        taskYIELD();
    };
exit:
    vTaskDelete(NULL);
}

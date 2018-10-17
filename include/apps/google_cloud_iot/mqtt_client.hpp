#ifndef OTBR_RTOS_MQTT_CLIENT_HPP_
#define OTBR_RTOS_MQTT_CLIENT_HPP_

#include "FreeRTOS.h"
#include "jwt.h"
#include "semphr.h"
#include "lwip/apps/mqtt.h"

namespace ot {
namespace app {

struct GoogleCloudIotClientCfg
{
    const char *mAddress;
    const char *mClientId;
    const char *mDeviceId;
    const char *mRegistryId;
    const char *mProjectId;
    const char *mRegion;
    const char *mRootCertificate;
    const char *mPrivKey;

    jwt_alg_t mAlgorithm;
};

class GoogleCloudIotMqttClient
{
public:
    GoogleCloudIotMqttClient(const GoogleCloudIotClientCfg &aConfig);

    int Connect();

    int Publish(const char *aTopi, const uint8_t *aMsg, size_t aMsgLength);

    ~GoogleCloudIotMqttClient();

private:
    static void mqttPubChanged(void *aArg, err_t aResult);

    static void mqttConnectChanged(mqtt_client_t *aClient, void *aArg, mqtt_connection_status_t aStatus);

    GoogleCloudIotClientCfg mConfig;

    struct mqtt_connect_client_info_t mClientInfo;
    mqtt_client_t *                   mMqttClient;
    mqtt_connection_status_t          mConnectResult;
};

} // namespace app
} // namespace ot

#endif

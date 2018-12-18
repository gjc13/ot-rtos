cmake_minimum_required (VERSION 3.7)

set(MINCRYPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/android/platform-system/core-mincrypt)

set(MINCRYPT_SRCS 
    ${MINCRYPT_DIR}/libmincrypt/sha.c
    ${MINCRYPT_DIR}/libmincrypt/sha256.c
)

add_library(mincrypt ${MINCRYPT_SRCS})

target_include_directories(mincrypt
    PUBLIC
        ${MINCRYPT_DIR}/include
)

set(MICRO_ECC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/micro-ecc/)

set(MICRO_ECC_SRCS ${MICRO_ECC_DIR}/micro-ecc/uECC.c)

add_library(micro_ecc ${MICRO_ECC_SRCS})

target_include_directories(micro_ecc
    PUBLIC
        ${MICRO_ECC_DIR}
)

target_compile_definitions(micro_ecc
    PUBLIC
        uECC_WORD_SIZE=4
        uECC_RNG_MAX_TRIES=2
        uECC_ENABLE_VLI_API=1
        uECC_SUPPORTS_secp256k1=0
        uECC_SUPPORT_COMPRESSED_POINT=0
)

set(NL_FAULT_INJECTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/nlfaultinjection/repo/)

add_library(nlfaultinjection ${NL_FAULT_INJECTION_DIR}/src/nlfaultinjection.cpp)

target_include_directories(nlfaultinjection
    PUBLIC
        ${NL_FAULT_INJECTION_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/nlassert/repo/include/
)
        
set(OPENWEAVE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo/src)

set(OPENWEAVE_SRCS
    ${OPENWEAVE_SRC_DIR}/device-manager/WeaveDeviceManager.cpp
    ${OPENWEAVE_SRC_DIR}/device-manager/WeavePlatformPersistedStorage.cpp
    #${OPENWEAVE_SRC_DIR}/inet/AsyncDNSResolverSockets.cpp
    #${OPENWEAVE_SRC_DIR}/inet/DNSResolver.cpp
    ${OPENWEAVE_SRC_DIR}/inet/EndPointBasis.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetFaultInjection.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetInterface.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetLayer.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetLayerBasis.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetTimer.cpp
    ${OPENWEAVE_SRC_DIR}/inet/InetUtils.cpp
    ${OPENWEAVE_SRC_DIR}/inet/IPAddress-StringFuncts.cpp
    ${OPENWEAVE_SRC_DIR}/inet/IPAddress.cpp
    ${OPENWEAVE_SRC_DIR}/inet/IPPrefix.cpp
    ${OPENWEAVE_SRC_DIR}/inet/TCPEndPoint.cpp
    ${OPENWEAVE_SRC_DIR}/inet/TunEndPoint.cpp
    ${OPENWEAVE_SRC_DIR}/inet/UDPEndPoint.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/ExchangeContext.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/HostPortList.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveBinding.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveCircularTLVBuffer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveConnection.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveConnectionTunnel.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveExchangeMgr.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveFabricState.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveGlobals.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveKeyIds.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveMessageLayer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveSecurityMgr-Malloc.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveSecurityMgr.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveServerBase.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveStats.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveTLVDebug.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveTLVReader.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveTLVUpdater.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveTLVUtilities.cpp
    ${OPENWEAVE_SRC_DIR}/lib/core/WeaveTLVWriter.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/echo/Current/WeaveEchoClient.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/echo/Current/WeaveEchoServer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/device-control/DeviceControl.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/device-description/DeviceDescription.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/device-description/DeviceDescriptionServer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/network-provisioning/NetworkInfo.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/network-provisioning/NetworkProvisioning.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/fabric-provisioning/FabricProvisioning.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/ApplicationKeysTrait.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/ApplicationKeysTraitDataSink.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveAccessToken.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveApplicationKeys.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveCASEEngine.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveCASEMessages.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveCert.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveDummyGroupKeyStore.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveKeyExport.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveKeyExportClient.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeavePASEEngine.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeavePasscodes.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeavePrivateKey.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveProvBundle.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveProvHash.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveSecurity.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveSecurityDebug.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveSig.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveTAKEEngine.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/WeaveToX509.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/security/X509ToWeave.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/status-report/StatusReportProfile.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/common/WeaveMessage.cpp
    ${OPENWEAVE_SRC_DIR}/lib/profiles/common/RetainedPacketBuffer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/CTRMode.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/DRBG.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/EllipticCurve-uECC.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/EllipticCurve.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/HashAlgos-MinCrypt.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/HKDF.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/HMAC.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/WeaveCrypto.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/crypto/WeaveRNG-NestDRBG.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/logging/DecodedIPPacket.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/logging/WeaveLogging.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/pairing-code/KryptonitePairingCodeUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/pairing-code/NevisPairingCodeUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/pairing-code/PairingCodeUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/verhoeff/Verhoeff.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/verhoeff/Verhoeff10.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/verhoeff/Verhoeff16.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/verhoeff/Verhoeff32.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/verhoeff/Verhoeff36.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/ASN1OID.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/ASN1Reader.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/ASN1Writer.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/Base64.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/ErrorStr.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/FibonacciUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/MathUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/NestCerts.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/nlargparser.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/NonProductionMarker.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/PersistedCounter.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/ProfileStringSupport.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/RandUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/SerializationUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/SerialNumberUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/StatusReportStr.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/TimeUtils.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/WeaveCounter.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/WeaveFaultInjection.cpp
    ${OPENWEAVE_SRC_DIR}/lib/support/WeaveNames.cpp
    ${OPENWEAVE_SRC_DIR}/ra-daemon/RADaemon.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemClock.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemError.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemLayer.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemFaultInjection.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemMutex.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemObject.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemPacketBuffer.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemStats.cpp
    ${OPENWEAVE_SRC_DIR}/system/SystemTimer.cpp
    ${OPENWEAVE_SRC_DIR}/warm/WarmCore.cpp
    ${OPENWEAVE_SRC_DIR}/test-apps/MockDDServer.cpp
    ${OPENWEAVE_SRC_DIR}/test-apps/DeviceDescOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread_assist_pair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openweave_adaption/AESBlockCipher-MbedTLS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openweave_adaption/entropy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openweave_adaption/NetworkProvisionServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openweave_adaption/FabricProvisionServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/openweave_adaption/GroupKeyMemoryStorage.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/CASEAuth.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/DeviceControlServer.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/DeviceDescriptionServer.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/DeviceIdentityTraitDataSource.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/EchoServer.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/FabricProvisioningServer.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/GeneralUtils.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/Globals.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/NetworkInfo.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/PersistedStorage.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/ServiceDirectoryManager.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/ServiceProvisioningServer.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/ServiceTunnelAgent.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/SystemEventSupport.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/SystemTimerSupport.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/TestDeviceIds.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/TimeSyncManager.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/TraitManager.cpp
    #${OPENWEAVE_SRC_DIR}/adaptations/device-layer/trait-support/weave/trait/description/DeviceIdentityTrait.cpp
)

add_library(openweave ${OPENWEAVE_SRCS})

target_include_directories(openweave 
    PUBLIC
        ${OPENWEAVE_SRC_DIR}/adaptations/device-layer/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/build/include
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/nlio/repo/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/nlassert/repo/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/openweave_adaption/config
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/
)

target_compile_definitions(openweave
    PUBLIC
        EXTERNAL_WEAVEDEVICEPLATFORMEVENT_HEADER=<openweave_adaption/WeaveDevicePlatformEvent.h>
        EXTERNAL_PLATFORMMANAGERIMPL_HEADER=<openweave_adaption/platform_manager_impl.hpp>
        EXTERNAL_WEAVEDEVICEPLATFORMCONFIG_HEADER=<WeaveDevicePlatformConfig.h>
        WEAVE_CONFIG_ENABLE_ARG_PARSER=1
    PRIVATE
        ULLONG_MAX=__LONG_LONG_MAX__*2ULL+1ULL
        _GNU_SOURCE
)

target_link_libraries(openweave
    PUBLIC
        lwip
        platform
        openthread
        micro_ecc
        mincrypt
    PRIVATE
        nlfaultinjection
)

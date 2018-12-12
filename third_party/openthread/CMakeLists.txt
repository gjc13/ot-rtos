cmake_minimum_required (VERSION 3.7)

include(ExternalProject)

ExternalProject_Add(ot
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo
    CONFIGURE_COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/repo && ./bootstrap
    BUILD_COMMAND sh -c "CPPFLAGS='-DMBEDTLS_BASE64_C -DMBEDTLS_OID_C -DMBEDTLS_PEM_PARSE_C -DMBEDTLS_X509_USE_C -DMBEDTLS_X509_CRT_PARSE_C -DNRF52840_XXAA' make -f ${CMAKE_CURRENT_SOURCE_DIR}/repo/examples/Makefile-nrf52840 JOINER=1 SNTP_CLIENT=1"
    INSTALL_COMMAND cmake -E echo "Skipping install step."
    EXCLUDE_FROM_ALL TRUE)

ExternalProject_Get_Property(ot BINARY_DIR)

add_library(nrf_cc310 STATIC IMPORTED)
set_property(TARGET nrf_cc310 PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/libraries/nrf_cc310/lib/libnrf_cc310_0.9.10.a)

add_library(openthread_diag STATIC IMPORTED)
set_property(TARGET openthread_diag PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-diag.a)

add_library(mbedcrypto STATIC IMPORTED)
set_property(TARGET mbedcrypto PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libmbedcrypto.a)
#target_link_libraries(mbedcrypto INTERFACE
#    openthread_nrf52840)

add_library(openthread_platform_utils STATIC IMPORTED)
set_property(TARGET openthread_platform_utils PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-platform-utils.a)

add_library(openthread_ftd STATIC IMPORTED)
set_property(TARGET openthread_ftd PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-ftd.a)

add_library(openthread_cli_ftd STATIC IMPORTED)
set_property(TARGET openthread_cli_ftd PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-cli-ftd.a)

add_library(openthread_nrf52840 STATIC IMPORTED)
set_property(TARGET openthread_nrf52840 PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-nrf52840.a)

add_library(openthread_nrf52840_sdk STATIC IMPORTED)
set_property(TARGET openthread_nrf52840_sdk PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-nrf52840-sdk.a)

add_library(openthread_nrf52840_softdevice_sdk STATIC IMPORTED)
set_property(TARGET openthread_nrf52840_softdevice_sdk PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/output/nrf52840/lib/libopenthread-nrf52840-softdevice-sdk.a)

add_library(openthread_nordicsemi INTERFACE)
target_include_directories(openthread_nordicsemi INTERFACE
    ${BINARY_DIR}/output/include
    ${BINARY_DIR}/build/nrf52840/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/cmsis
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/dependencies
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/drivers/clock
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/libraries/app_error
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/libraries/crypto
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/libraries/nrf_cc310/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/nrfx
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/nrfx/drivers/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/nrfx/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/NordicSemiconductor/nrfx/mdk)

target_compile_definitions(openthread_nordicsemi INTERFACE
    CONFIG_GPIO_AS_PINRESET
    NRF52840_XXAA)

add_dependencies(openthread_nordicsemi ot)

add_library(openthread INTERFACE)
target_link_libraries(openthread INTERFACE
    openthread_cli_ftd
    openthread_diag
    openthread_ftd
    mbedcrypto
    openthread_nrf52840
    openthread_nrf52840_sdk
    openthread_nrf52840_softdevice_sdk
    openthread_platform_utils
    openthread_nordicsemi
    nrf_cc310)

target_compile_options(openthread INTERFACE
    -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"
    -DMBEDTLS_USER_CONFIG_FILE=\"nrf52840-mbedtls-config.h\"
    -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE=\"openthread-core-nrf52840-config.h\"
    -DMBEDTLS_BASE64_C
    -DMBEDTLS_OID_C
    -DMBEDTLS_PEM_PARSE_C
    -DMBEDTLS_X509_USE_C
    -DMBEDTLS_X509_CRT_PARSE_C
    -DNRF52840_XXAA)

target_include_directories(openthread INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/examples/platforms
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/examples/platforms/nrf52840
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/mbedtls
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/mbedtls/repo/include
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/third_party/mbedtls/repo/include/mbedtls)

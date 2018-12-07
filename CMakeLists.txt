cmake_minimum_required (VERSION 3.7)

project(openthread_freertos)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ggdb")

#XXX: better way to manage config headers
add_library(mbedtls-config INTERFACE)
target_include_directories(mbedtls-config
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cfgs
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/third_party/NordicSemiconductor/libraries/crypto/
)
target_compile_definitions(mbedtls-config
    INTERFACE
        MBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"
        MBEDTLS_USER_CONFIG_FILE=\"nrf52840-mbedtls-config.h\"
)

add_library(openthread-config INTERFACE)
target_include_directories(openthread-config
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/platforms/nrf52840
)
target_compile_definitions(openthread-config
    INTERFACE
        _GNU_SOURCE
        ENABLE_FEM=1
        NRF_802154_PROJECT_CONFIG=\"platform-config.h\"
        OPENTHREAD_CONFIG_LOG_PLATFORM=1
        OPENTHREAD_FTD=1
        OPENTHREAD_PROJECT_CORE_CONFIG_FILE=\"openthread-core-nrf52840-config.h\"
        SPINEL_PLATFORM_HEADER=\"spinel_platform.h\"
)

add_library(nrf-sdk-config INTERFACE)
target_include_directories(nrf-sdk-config
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cfgs
)
target_compile_definitions(nrf-sdk-config
    INTERFACE
        FREERTOS
        _GNU_SOURCE
        CONFIG_GPIO_AS_PINRESET
        RAAL_SINGLE_PHY=1
        BOARD_PCA10056
        BSP_DEFINES_ONLY
        FLOAT_ABI_HARD
        NRF52840_XXAA
)

add_subdirectory(third_party/freertos)
add_subdirectory(third_party/nRF5_SDK)
add_subdirectory(third_party/openthread)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/libjwt)
add_subdirectory(third_party/lwip)

set(OT_PLATFORM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/nrf52840)
set(OT_PLATFORM_SRCS
    ${OT_PLATFORM_SRC_DIR}/alarm.c
    ${OT_PLATFORM_SRC_DIR}/diag.c
    ${OT_PLATFORM_SRC_DIR}/fem.c
    ${OT_PLATFORM_SRC_DIR}/flash.c
    ${OT_PLATFORM_SRC_DIR}/flash_nosd.c
    ${OT_PLATFORM_SRC_DIR}/logging.c
    ${OT_PLATFORM_SRC_DIR}/misc.c
    ${OT_PLATFORM_SRC_DIR}/netif.cpp
    ${OT_PLATFORM_SRC_DIR}/radio.c
    ${OT_PLATFORM_SRC_DIR}/random.c
    ${OT_PLATFORM_SRC_DIR}/spi-slave.c
    ${OT_PLATFORM_SRC_DIR}/spi-slave.c
    ${OT_PLATFORM_SRC_DIR}/system.c
    ${OT_PLATFORM_SRC_DIR}/temp.c
    ${OT_PLATFORM_SRC_DIR}/uart.c
)
add_library(ot_platform_nrf52840 ${OT_PLATFORM_SRCS})
target_include_directories(ot_platform_nrf52840
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_link_libraries(ot_platform_nrf52840
    PUBLIC
        openthread
    PRIVATE
        ot_freertos_app
        nrf_driver
        nrf_crypto
        nrf_radio
        lwip
)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(OT_FREERTOS_COMMON_SRCS
    ${SRC_DIR}/common/default_uart.c
    ${SRC_DIR}/common/entropy_utils.c
    ${SRC_DIR}/common/uart_io.c
)

add_library(ot_freertos_common ${OT_FREERTOS_COMMON_SRCS})

target_include_directories(ot_freertos_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ot_freertos_common
    PUBLIC
        mbedtls
        freertos
        freertos-portable
        nrf_driver
        openthread_platform_api
)

set(OT_FREERTOS_APP_SRCS
    ${SRC_DIR}/apps/google_cloud_iot/mqtt_client.cpp
    ${SRC_DIR}/apps/misc/nat64_utils.c
    ${SRC_DIR}/apps/misc/time_ntp.cpp
    ${SRC_DIR}/openthread/openthread_freertos.c
)

add_library(ot_freertos_app ${OT_FREERTOS_APP_SRCS})

target_include_directories(ot_freertos_app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ot_freertos_app
    PUBLIC
        openthread
        ot_freertos_common
        jansson
        libjwt
        lwip
        ot_platform_nrf52840
        mbedtls
)

add_executable(ot_cli_nrf52840 ${SRC_DIR}/examples/ot_cli/main.c)

target_link_libraries(ot_cli_nrf52840
    PUBLIC
        ot_freertos_app
)

target_compile_definitions(ot_cli_nrf52840
    PUBLIC
        __HEAP_SIZE=8192
        __STACK_SIZE=8192
)

#special link script
set_target_properties(ot_cli_nrf52840 PROPERTIES LINK_DEPENDS ${SRC_DIR}/platforms/nrf52840/gcc_nrf52.ld)
set_target_properties(ot_cli_nrf52840 PROPERTIES LINK_FLAGS "-L ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nRF5_SDK/repo/modules/nrfx/mdk/ -T ${SRC_DIR}/platforms/nrf52840/gcc_nrf52.ld -lc -lnosys -lm -lstdc++")

#build hex file
add_custom_command(OUTPUT ot_cli_nrf52840.hex
    COMMAND arm-none-eabi-objcopy -O ihex ot_cli_nrf52840 ot_cli_nrf52840.hex
    DEPENDS ot_cli_nrf52840
)
add_custom_target(ot_cli_nrf52840_hex ALL DEPENDS ot_cli_nrf52840.hex)

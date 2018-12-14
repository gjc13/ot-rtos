cmake_minimum_required (VERSION 3.7)

project(openthread_freertos)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_RELEASE "-Os")

set(FIRST_PARTY_COMPILE_FLAGS 
   -Wall
   -Wextra
   -Wshadow
   -Werror
   $<$<COMPILE_LANGUAGE:CXX>:
       -std=gnu++98
       -Wno-c++14-compat
       -fno-exceptions
       -fno-rtti
   >
)

add_subdirectory(third_party/freertos)
add_subdirectory(third_party/freertos_portable)
add_subdirectory(third_party/freertos-addons)
add_subdirectory(third_party/openthread)
add_subdirectory(third_party/openweave-core)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/libjwt)
add_subdirectory(third_party/lwip)
add_subdirectory(third_party/mbedtls)

add_subdirectory(src/apps/test)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(otr_common_utils
    ${SRC_DIR}/common/entropy_utils.c
)

target_include_directories(otr_common_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(otr_common_utils
    PUBLIC
        mbedcrypto
    PRIVATE
        openthread
)

target_compile_options(otr_common_utils
    PRIVATE
        ${FIRST_PARTY_COMPILE_FLAGS}
)

add_library(otr_core
    ${SRC_DIR}/openthread/netif.cpp
    ${SRC_DIR}/openthread/uart_lock.c
    ${SRC_DIR}/openthread/openthread_freertos.c
    ${SRC_DIR}/openthread/uart_lock.c
)

target_include_directories(otr_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(otr_core
    PUBLIC
        openthread
        freertos
        mbedtls
        lwip
)
        

add_library(otr_frameworks
    ${SRC_DIR}/apps/misc/nat64_utils.c
    ${SRC_DIR}/apps/misc/time_ntp.cpp
)

target_link_libraries(otr_frameworks
    PUBLIC
        otr_core
        jansson
        libjwt
        lwip
        openthread
        openweave
)

target_compile_options(otr_frameworks
    PRIVATE
        ${FIRST_PARTY_COMPILE_FLAGS}
)

add_executable(ot_cli_${PLATFORM_NAME} ${SRC_DIR}/examples/ot_cli/main.c)

target_link_libraries(ot_cli_${PLATFORM_NAME}
    PUBLIC
        test_app
)

if (${PLATFORM_NAME} STREQUAL nrf52)
    target_compile_definitions(ot_cli_${PLATFORM_NAME}
        PUBLIC
            __HEAP_SIZE=8192
            __STACK_SIZE=8192
    )

    #special link script
    set_target_properties(ot_cli_${PLATFORM_NAME} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/examples/platforms/nrf52840/nrf52840.ld)
    set_target_properties(ot_cli_${PLATFORM_NAME} PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/examples/platforms/nrf52840/nrf52840.ld -lc -lnosys -lm -lstdc++")

    #build hex file
    add_custom_command(OUTPUT ot_cli_${PLATFORM_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O ihex ot_cli_${PLATFORM_NAME} ot_cli_${PLATFORM_NAME}.hex
        DEPENDS ot_cli_${PLATFORM_NAME}
    )
    add_custom_target(ot_cli_nrf52_hex ALL DEPENDS ot_cli_${PLATFORM_NAME}.hex)
endif()

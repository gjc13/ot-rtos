cmake_minimum_required (VERSION 3.7)

project(openthread_freertos)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_RELEASE "-Os")

add_subdirectory(third_party/freertos)
add_subdirectory(third_party/openthread)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/libjwt)
add_subdirectory(third_party/lwip)
add_subdirectory(third_party/freertos_portable)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(FRAMEWORKS_SRCS
    ${SRC_DIR}/apps/misc/nat64_utils.c
    ${SRC_DIR}/apps/misc/time_ntp.cpp
    ${SRC_DIR}/common/entropy_utils.c
    ${SRC_DIR}/openthread/netif.cpp
    ${SRC_DIR}/openthread/openthread_freertos.c
)

add_library(platform ${FRAMEWORKS_SRCS})

target_include_directories(platform
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(platform
    PUBLIC
        lwip
        freertos_portable
        openthread
)

add_library(frameworks INTERFACE)
target_link_libraries(frameworks INTERFACE
        jansson
        libjwt
        platform)

target_compile_options(platform
    PRIVATE
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

add_subdirectory(src/apps/test)

add_executable(ot_cli_nrf52840 ${SRC_DIR}/examples/ot_cli/main.c)

target_link_libraries(ot_cli_nrf52840
    PUBLIC
        test_app
)

target_compile_definitions(ot_cli_nrf52840
    PUBLIC
        __HEAP_SIZE=8192
        __STACK_SIZE=8192
)

#special link script
set_target_properties(ot_cli_nrf52840 PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/examples/platforms/nrf52840/nrf52840.ld)
set_target_properties(ot_cli_nrf52840 PROPERTIES LINK_FLAGS "-L ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nRF5_SDK/repo/modules/nrfx/mdk/ -T ${CMAKE_CURRENT_SOURCE_DIR}/third_party/openthread/repo/examples/platforms/nrf52840/nrf52840.ld -lc -lnosys -lm -lstdc++")

#build hex file
add_custom_command(OUTPUT ot_cli_nrf52840.hex
    COMMAND arm-none-eabi-objcopy -O ihex ot_cli_nrf52840 ot_cli_nrf52840.hex
    DEPENDS ot_cli_nrf52840
)
add_custom_target(ot_cli_nrf52840_hex ALL DEPENDS ot_cli_nrf52840.hex)

cmake_minimum_required (VERSION 3.7)

project (nrf_driver)

enable_language(ASM)

#use gcc rather than as
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/mdk/gcc_startup_nrf52840.S
    PROPERTIES LANGUAGE C)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/mdk/gcc_startup_nrf52840.S
    PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

set(NRF_DRIVER_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/mdk/gcc_startup_nrf52840.S
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/log/src/nrf_log_frontend.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/log/src/nrf_log_str_formatter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/boards/boards.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util/app_error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util/app_error_handler_gcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util/app_error_weak.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/timer/app_timer_freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/uart/app_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/fifo/app_fifo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util/app_util_platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util/nrf_assert.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/atomic/nrf_atomic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/balloc/nrf_balloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/fprintf/nrf_fprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/fprintf/nrf_fprintf_format.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/memobj/nrf_memobj.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/ringbuf/nrf_ringbuf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/strerror/nrf_strerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/integration/nrfx/legacy/nrf_drv_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/integration/nrfx/legacy/nrf_drv_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/integration/nrfx/legacy/nrf_drv_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/src/nrfx_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/src/nrfx_power_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/src/prs/nrfx_prs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/src/nrfx_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/src/nrfx_uarte.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/mdk/system_nrf52840.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/hal/nrf_nvmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/segger_rtt/SEGGER_RTT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/segger_rtt/SEGGER_RTT_printf.c
)

add_library(nrf_driver ${NRF_DRIVER_SRCS})

target_link_libraries(nrf_driver
    PUBLIC
        nrf-sdk-config
)

target_include_directories(nrf_driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/mdk
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/fifo
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/strerror
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/toolchain/cmsis/include
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/util
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/balloc
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/ringbuf
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/hal
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/bsp
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/uart
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/log
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/experimental_section_vars
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/integration/nrfx/legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/delay
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/integration/nrfx
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/drivers_nrf/nrf_soc_nosd
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/atomic
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/boards
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/memobj
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/modules/nrfx/drivers/include
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/segger_rtt
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/components/libraries/log/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/fprintf
)

add_library(nrf_cc310 STATIC IMPORTED GLOBAL)
add_library(nrf_cc310_interface INTERFACE)

target_include_directories(nrf_cc310_interface
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/nrf_cc310/include
)

set_property(TARGET nrf_cc310 PROPERTY IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/nrf_cc310/lib/libnrf_cc310_0.9.10.a
)

set(FREERTOS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/GCC/nrf52/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52/port_cmsis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c
)

add_library(freertos-portable ${FREERTOS_SRCS})

target_include_directories(freertos-portable
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/GCC/nrf52
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52
)

target_link_libraries(freertos-portable
    PUBLIC
        nrf_driver
        freertos
)

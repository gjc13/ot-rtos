cmake_minimum_required (VERSION 3.7)

set(FREERTOS_PORTABLE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/GCC/nrf52/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52/port_cmsis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c
)

add_library(freertos_portable ${FREERTOS_PORTABLE_SRCS})

target_include_directories(freertos_portable PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/GCC/nrf52
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/external/freertos/portable/CMSIS/nrf52
)

target_link_libraries(freertos_portable
    PUBLIC
        freertos
        openthread
)

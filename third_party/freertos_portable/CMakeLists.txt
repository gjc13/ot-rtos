set(FREERTOS_PORTABLE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/nrf52/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nrf52/port_cmsis.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nrf52/port_cmsis_systick.c
)

add_library(freertos_portable ${FREERTOS_PORTABLE_SRCS})

target_include_directories(freertos_portable PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/nrf52)

target_link_libraries(freertos_portable
    PUBLIC
        freertos
)

target_compile_options(freertos_portable
    PRIVATE
       -Wall
       -Wextra
       -Wshadow
       -Werror
)

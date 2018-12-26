set(FREERTOS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/portable/MemMang/heap_3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/timers.c)

add_library(freertos ${FREERTOS_SRCS})

target_include_directories(freertos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(freertos
    PUBLIC
        freertos_portable
)

target_compile_options(freertos
    PRIVATE
       -Wall
       -Wextra
       -Wshadow
       -Werror)

add_library(freertos_port_hdrs INTERFACE)

target_include_directories(freertos_port_hdrs
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/FreeRTOS/Source/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)


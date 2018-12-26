set(FREERTOS_Linux_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/repo/Linux/portable/GCC/Linux)

if (${PLATFORM_NAME} STREQUAL linux OR ${PLATFORM_NAME} STREQUAL linux_radio)
    add_library(freertos_portable_linux ${FREERTOS_Linux_PORT_DIR}/port.c)

    target_include_directories(freertos_portable_linux
        PUBLIC
            ${FREERTOS_Linux_PORT_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(freertos_portable_linux
        PUBLIC
            freertos_port_hdrs
            pthread
    )

    add_library(freertos_portable INTERFACE)

    target_link_libraries(freertos_portable
        INTERFACE
            freertos_portable_linux
    )
endif()

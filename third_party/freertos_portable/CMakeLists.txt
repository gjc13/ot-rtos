if (${PLATFORM_NAME} STREQUAL nrf52)
    add_subdirectory(nrf52)

    add_library(freertos_portable INTERFACE)

    target_link_libraries(freertos_portable
        INTERFACE
            freertos_portable_nrf52
    )
endif()

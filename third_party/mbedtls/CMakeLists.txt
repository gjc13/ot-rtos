cmake_minimum_required(VERSION 3.7)

set(ENABLE_TESTING OFF CACHE BOOL "Disable mbedtls test")
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable mbetls program")

add_subdirectory(repo)

#unforunately mbedtls used include_directories which is not visisble outside
target_include_directories(mbedcrypto
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/include/
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:openthread_config,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:mbedtls_platform_config,INTERFACE_INCLUDE_DIRECTORIES>
)

target_compile_definitions(mbedcrypto
    PUBLIC
        MBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"
        ENABLE_ECDHE_RSA=1
        $<TARGET_PROPERTY:openthread_config,INTERFACE_COMPILE_DEFINITIONS>
        $<TARGET_PROPERTY:mbedtls_platform_config,INTERFACE_COMPILE_DEFINITIONS>
)

add_dependencies(mbedcrypto openthread_config)

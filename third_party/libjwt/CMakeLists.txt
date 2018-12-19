cmake_minimum_required (VERSION 3.7)

project (libjwt)

write_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h "")

set(LIBJWT_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/libjwt/base64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/libjwt/jwt-mbedtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/libjwt/jwt.c
)

add_library(libjwt ${LIBJWT_SRC_FILES})

target_compile_options(libjwt PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Werror
    -Wno-unused-parameter
    -Wno-pointer-sign
    -Wno-unused-variable
    -Wno-type-limits
    -Wno-unused-function)

target_include_directories(libjwt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libjwt
    PRIVATE
        jansson
        platform
)

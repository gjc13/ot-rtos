cmake_minimum_required (VERSION 3.7)

project (jansson)

set(JANSSON_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/dump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/hashtable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/hashtable_seed.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/load.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/pack_unpack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/strbuffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/strconv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/utf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/src/value.c
)

add_library(jansson ${JANSSON_SRC_FILES})

target_compile_options(jansson PRIVATE
    -Wno-unused-parameter
    -Wno-unused-function)

target_compile_definitions(jansson
    PUBLIC
        HAVE_CONFIG_H
)

target_include_directories(jansson
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/repo/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

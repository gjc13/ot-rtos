cmake_minimum_required (VERSION 3.7)

add_library(test_app
    ${CMAKE_CURRENT_SOURCE_DIR}/http.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mqtt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/user.c)

target_link_libraries(test_app
    PUBLIC
        frameworks)

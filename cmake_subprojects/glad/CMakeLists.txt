cmake_minimum_required(VERSION 3.10)
project(glad)

set(GLAD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/src)
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/include)

add_library(glad STATIC
        ${GLAD_SOURCE_DIR}/glad.c)

target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

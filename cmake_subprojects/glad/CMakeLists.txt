cmake_minimum_required(VERSION 3.15)
project(glad)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/src/*.c)

add_library(glad ${GLAD_SOURCES})

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/include)
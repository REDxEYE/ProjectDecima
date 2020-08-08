cmake_minimum_required(VERSION 3.15)
project(glad)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/src/*.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_library(glad SHARED ${GLAD_SOURCES})
set_target_properties(glad PROPERTIES PREFIX "")

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/glad/include)
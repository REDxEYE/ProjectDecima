cmake_minimum_required(VERSION 3.10)
project(oodle)

set(OODLE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/oodle)
set(OODLE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/oodle)

add_library(oodle STATIC
        ${OODLE_SOURCE_DIR}/Kraken.cpp)

target_include_directories(oodle PUBLIC ${OODLE_INCLUDE_DIR})

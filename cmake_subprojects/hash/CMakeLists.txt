cmake_minimum_required(VERSION 3.10)
project(hash)

set(HASH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/hash)
set(HASH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/hash)

add_library(hash STATIC
        ${HASH_SOURCE_DIR}/md5.c
        ${HASH_SOURCE_DIR}/MurmurHash3.cpp)

target_include_directories(hash PUBLIC ${HASH_INCLUDE_DIR})

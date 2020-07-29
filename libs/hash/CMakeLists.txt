cmake_minimum_required(VERSION 3.15)
project(HashLib)

set(CMAKE_CXX_STANDARD 17)

set(HASHLIB_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/

        PARENT_SCOPE
        )

add_library(HashLib md5.c MurmurHash3.cpp)
target_link_libraries(HashLib -static)
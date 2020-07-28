cmake_minimum_required(VERSION 3.15)
project(oozLib)

set(CMAKE_CXX_STANDARD 17)

set(OOZLIB_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/

        PARENT_SCOPE
        )

add_library(oozLib Kraken.cpp)
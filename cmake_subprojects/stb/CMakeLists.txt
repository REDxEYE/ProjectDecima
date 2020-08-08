cmake_minimum_required(VERSION 3.15)
project(stb)
set(CMAKE_CXX_STANDARD 17)

set(STB_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/stb
        CACHE INTERNAL ""
        )

cmake_minimum_required(VERSION 3.15)
project(HashLib)

set(CMAKE_CXX_STANDARD 17)

set(HASHLIB_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/

        PARENT_SCOPE
        )

add_library(HashLib md5.c MurmurHash3.cpp)

#if (DEBUG_MODE)
#    target_compile_options(HashLib PUBLIC -g -fno-omit-frame-pointer -gdwarf-2)
#    message("${PROJECT_NAME} debug mode")
#else()
#    target_compile_options(HashLib PUBLIC -ftree-vectorize -ftree-vectorizer-verbose=2)
#    message("${PROJECT_NAME} release mode")
#endif (DEBUG_MODE)

target_link_libraries(HashLib -static)
cmake_minimum_required(VERSION 3.15)
project(oozLib)

set(CMAKE_CXX_STANDARD 17)

set(OOZLIB_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/

        PARENT_SCOPE
        )


set(GCC_COVERAGE_COMPILE_FLAGS -Wpsabi)



SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_library(oozLib Kraken.cpp)

#if (DEBUG_MODE)
#    target_compile_options(oozLib PUBLIC -g -fno-omit-frame-pointer -gdwarf-2)
#    message("${PROJECT_NAME} debug mode")
#else()
#    target_compile_options(oozLib PUBLIC -ftree-vectorize -ftree-vectorizer-verbose=2)
#    message("${PROJECT_NAME} release mode")
#endif (DEBUG_MODE)

target_link_libraries(oozLib -static)
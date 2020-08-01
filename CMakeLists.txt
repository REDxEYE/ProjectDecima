cmake_minimum_required(VERSION 3.15)
project(ProjectDS)

if (DEBUG_MODE)
    set(GCC_COVERAGE_COMPILE_FLAGS "-g -fno-omit-frame-pointer -gdwarf-2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    message("${PROJECT_NAME} debug mode")
else ()
    #-fopt-info-vec-optimized -fopt-info-inline-optimized
    set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -ffast-math -msse2 -mfpmath=sse -mavx")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    message("${PROJECT_NAME} release mode")
endif (DEBUG_MODE)


add_subdirectory(libs/hash)
add_subdirectory(libs/ooz)
add_subdirectory(libs/LGLE)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjectDS
        src/main.cpp
        src/decima_archive.cpp
        src/file_types/prefetch.cpp
        src/archive_array.cpp
        src/utils.cpp
        src/file_types/prefetch.cpp
        src/memory_stream.cpp
        src/file_tree.cpp src/file_types/core.cpp)

target_include_directories(ProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC} ${LGLE_INCLUDE} ${LGLE_LIB_INCLUDE})


target_link_libraries(ProjectDS PUBLIC
        HashLib oozLib LGLE
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)



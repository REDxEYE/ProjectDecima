cmake_minimum_required(VERSION 3.15)
project(ProjectDS)
set(CMAKE_CXX_STANDARD 17)

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


include(ProjectDS.cmake)
include(QuickHash.cmake)
include(ArchiveTest.cmake)
include(no_lgle_ProjectDS.cmake)



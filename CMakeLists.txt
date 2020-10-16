cmake_minimum_required(VERSION 3.10)
project(ProjectDS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "64-bit platform is required")
endif ()

add_subdirectory(cmake_subprojects/hash)
add_subdirectory(cmake_subprojects/glad)
add_subdirectory(cmake_subprojects/imgui)
add_subdirectory(libs/glfw)

include(ProjectDS.cmake)
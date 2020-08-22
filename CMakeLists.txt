cmake_minimum_required(VERSION 3.15)
project(ProjectDS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(cmake_subprojects/hash)
add_subdirectory(cmake_subprojects/glad)
add_subdirectory(cmake_subprojects/imgui)
add_subdirectory(cmake_subprojects/oodle)
add_subdirectory(libs/glfw)

include(ProjectDS.cmake)
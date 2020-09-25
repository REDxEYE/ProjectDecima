cmake_minimum_required(VERSION 3.10)
project(imgui)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/imgui)
set(IMGUI_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/imgui/examples
        ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(imgui STATIC
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp
        ${IMGUI_SOURCE_DIR}/examples/imgui_impl_glfw.cpp)

target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_link_libraries(imgui PRIVATE opengl32 glfw glad)
cmake_minimum_required(VERSION 3.15)
project(imgui)
set(CMAKE_CXX_STANDARD 17)

set(IMGUI_SRC_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/imgui
        CACHE INTERNAL ""
        )

set(IMGUI_INCLUDE_DIR
        ${IMGUI_SRC_DIR}
        ${IMGUI_SRC_DIR}/examples

        CACHE INTERNAL ""
        )

set(IMGUI_SOURCES
        ${IMGUI_SRC_DIR}/imgui.cpp
        ${IMGUI_SRC_DIR}/imgui_draw.cpp
        ${IMGUI_SRC_DIR}/imgui_widgets.cpp
        CACHE INTERNAL ""
        )



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_library(imgui SHARED ${IMGUI_SOURCES})

set_target_properties(imgui PROPERTIES PREFIX "")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

target_link_libraries(imgui
        -static-libgcc -static-libstdc++ -static
        )
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} lib/glfw/include lib/glad/include)


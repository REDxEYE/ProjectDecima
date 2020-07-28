cmake_minimum_required(VERSION 3.15)
project(LGLE)
set(CMAKE_CXX_STANDARD 17)


set(LGLE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/object/shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/object/model/model_indexed.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/application/opengl_backend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/application/application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/application/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/application/layer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/object/texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_addons/im_canvas.cpp
        CACHE INTERNAL ""
        )

set(LGLE_LIB_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/include
        CACHE INTERNAL ""
        )

set(LGLE_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        CACHE INTERNAL ""
        )

set(LGLE_LINK_OPTS
        glad glfw imgui
        -static-libgcc -static-libstdc++
        -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
        CACHE INTERNAL ""
        )


add_executable(LGLE
        ${LGLE_SRC}
        src/main.cpp
        )


add_subdirectory(lib/glm)
add_subdirectory(lib/glfw)

file(GLOB_RECURSE IMGUI_SOURCES lib/imgui/src/*.cpp)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC lib/imgui/include lib/glfw/include lib/glad/include lib/stb)

file(GLOB_RECURSE GLAD_SOURCES lib/glad/src/*.c)
add_library(glad ${GLAD_SOURCES})
target_include_directories(glad PUBLIC lib/glad/include)


target_include_directories(LGLE PUBLIC include
        ${LGLE_LIB_INCLUDE})


target_link_libraries(LGLE PUBLIC
        ${LGLE_LINK_OPTS})



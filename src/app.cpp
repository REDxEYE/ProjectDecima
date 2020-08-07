//
// Created by i.getsman on 06.08.2020.
//

#include <functional>
#include <iostream>

#include "app.hpp"

#include "utils.h"

App::App(std::pair<uint32_t, uint32_t> window_size, std::string title) {
    m_win_info.m_win_width = window_size.first;
    m_win_info.m_win_height = window_size.second;
    m_win_info.m_title = std::move(title);
}

void App::init() {
    log("App", "Initialization");
    init_glfw();
    init_opengl();
    init_user();
    log("App", "Done");
}

void App::run() {
    while (!glfwWindowShouldClose(m_window)) {
        auto ts = glfwGetTime() - m_prev_time;
        update(ts);
        m_prev_time = glfwGetTime();
    }
}

void App::init_glfw() {
    log("App::GLFW", "Initialization");
    glfwInit();
    m_window = glfwCreateWindow(m_win_info.m_win_width, m_win_info.m_win_height, m_win_info.m_title.c_str(),
        nullptr, nullptr);
    auto binded_error_callback = std::bind(&App::glfw_error_handler, this,
        std::placeholders::_1,
        std::placeholders::_2);
    glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(&binded_error_callback));
    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_FALSE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* hwnd, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow* hwnd, int maximized) {
        int width, height;
        glfwGetWindowSize(hwnd, &width, &height);
        glViewport(0, 0, width, height);
    });
    log("App::GLFW", "Done");
}

void App::glfw_error_handler(int error, const char* message) {
    throw std::runtime_error("GLFW error: " + std::to_string(error) + ": " + message);
}

void App::init_opengl() {
    log("App GLAD", "Initialization");
    gladLoadGL();
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    log("App GLAD", "Done");
}

void App::update(double ts) {
    begin_frame();
    update_user(ts);
    end_frame();
}

void App::clear_window(float r, float g, float b, float a, uint32_t flags) {
    glClear(flags);
    glClearColor(r, g, b, a);
}

void App::begin_frame() {
    clear_window();
    begin_frame_user();
}

void App::end_frame() {
    end_frame_user();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void App::init_user() {
}

void App::update_user(double ts) {
}

void App::begin_frame_user() {
}

void App::end_frame_user() {
}

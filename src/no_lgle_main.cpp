//
// Created by i.getsman on 06.08.2020.
//
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "glad/glad.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include <string>
#include <stdexcept>


static void glfw_error_handler(int error, const char* message) {
    throw std::runtime_error("GLFW error: " + std::to_string(error) + ": " + message);
}


GLFWwindow* init_glfw(std::pair<uint32_t, uint32_t> window_size, const std::string& title) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(window_size.first, window_size.second, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* hwnd, int width, int height) { ;
        glViewport(0, 0, width, height);
    });
    glfwSetWindowMaximizeCallback(window, [](GLFWwindow* hwnd, int maximized) {
        int width, height;
        glfwGetWindowSize(hwnd, &width, &height);
        glViewport(0, 0, width, height);
    });

    return window;
}

void init_glad() {
    gladLoadGL();
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void init_imgui(GLFWwindow* window, bool multi_viewport) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    if (multi_viewport) {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui_ImplOpenGL3_NewFrame();


}

void imgui_new_frame() {
//    for(auto viewport: ImGui::GetPlatformIO().Viewports) {
//
//        ImGui::GetPlatformIO().Platform_SetWindowAlpha(viewport,0.5);
//    }
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void imgui_render_frame() {
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

inline void clear(float r = 0, float g = 0, float b = 0, float a = 1,
                         uint32_t flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) {
    glClear(flags);
    glClearColor(r, g, b, a);
}

void user_update(GLFWwindow* window){
    imgui_new_frame();
    {

        ImGui::SetNextWindowBgAlpha(0.1f);
        ImGui::Begin("DEBUG");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        ImGui::End();
    }
    imgui_render_frame();
}


int main() {
    auto* window = init_glfw({1280, 720}, "Project Decima");
    init_glad();
    init_imgui(window, false);
    while (!glfwWindowShouldClose(window)) {
        clear();

        user_update(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
//
// Created by MED45 on 14.01.2020.
//
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include "application/opengl_backend.h"
#include "utils.h"

void
error_callback(GLenum source, GLenum type,
               GLuint id, GLenum severity,
               GLsizei length, const GLchar* message, const void*) {
    std::string error_source;
    std::string error_type;
    std::string error_severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            error_source = "DEBUG_SOURCE_API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            error_source = "DEBUG_SOURCE_WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            error_source = "DEBUG_SOURCE_SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            error_source = "DEBUG_SOURCE_THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            error_source = "DEBUG_SOURCE_APPLICATION";
            break;
        default:
            error_source = "DEBUG_SOURCE_OTHER";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            error_type = "DEBUG_TYPE_ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            error_type = "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            error_type = "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            error_type = "DEBUG_TYPE_PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            error_type = "DEBUG_TYPE_PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            error_type = "DEBUG_TYPE_MARKER";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            error_type = "DEBUG_TYPE_PUSH_GROUP";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            error_type = "DEBUG_TYPE_POP_GROUP";
            break;
        default:
            error_type = "DEBUG_TYPE_OTHER";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            error_severity = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            error_severity = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            error_severity = "LOW";
            break;
        default:
            error_severity = "NOTIFICATION";
            break;
    }

    printf("[OPENGL : %s : %s] %s : %s\n", error_severity.c_str(), error_source.c_str(), error_type.c_str(), message);
}

omc::opengl_backend::opengl_backend(uint32_t initial_width, uint32_t initial_height, const std::string &app_title) :
        window_width(initial_width), window_height(initial_height) {
    glfwInit();
    window = glfwCreateWindow(window_width, window_height, app_title.c_str(), nullptr, nullptr);
//    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    init_glad();
    init_imgui();

    mouse_prev = get_mouse_pos();


}

void omc::opengl_backend::init_glad() {
    gladLoadGL();
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(GLDEBUGPROC(error_callback), nullptr);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void omc::opengl_backend::update_window() {
    mouse_delta.x = get_mouse_pos().x - mouse_prev.x;
    mouse_delta.y = mouse_prev.y - get_mouse_pos().y;
    mouse_prev = get_mouse_pos();

}


void omc::opengl_backend::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(this->get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui_ImplOpenGL3_NewFrame();

}

void omc::opengl_backend::imgui_new_frame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void omc::opengl_backend::imgui_render_frame() {
    // Rendering

    // trying find what function causes gl object creation spam
//    printf("TEST\n");
    ImGui::Render();
//    printf("TEST2\n");
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//    printf("TEST3\n");

}

void omc::opengl_backend::resize_viewport(uint32_t width, uint32_t height) {
    glViewport(0, 0, window_width, window_height);
    window_width = width;
    window_height = height;
}

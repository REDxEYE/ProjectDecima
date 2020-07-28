#include <glad/glad.h>
#include <application/window.hpp>

#include <stdexcept>
#include <utility>

static void glfw_error_handler(int error, const char* message) {
    throw std::runtime_error("GLFW error: " + std::to_string(error) + ": " + message);
}

omc::window::window(omc::window::window_info info) : gl_backend(info.width,
                                                                info.height,
                                                                info.title) {
    this->info = std::move(info);
    glfwSetErrorCallback(glfw_error_handler);
    set_vsync(this->info.vsync);
    glfwSetWindowUserPointer(this->gl_backend.get_window(), this);
    glfwMakeContextCurrent(this->gl_backend.get_window());
    glfwSetWindowSizeCallback(this->gl_backend.get_window(), [](GLFWwindow* hwnd, int width, int height) {
        auto window = (omc::window*) glfwGetWindowUserPointer(hwnd);
        window->resize_callback(width, height);
    });
    glfwSetWindowMaximizeCallback(this->gl_backend.get_window(), [](GLFWwindow* hwnd, int maximized) {
        auto window = (omc::window*) glfwGetWindowUserPointer(hwnd);
        int width,height;
        glfwGetWindowSize(window->get_handle(),&width,&height);
        window->resize_callback(width, height);
    });
    // TODO: Events
}

void omc::window::resize_callback(uint32_t width, uint32_t height) {
    this->info.width = width;
    this->info.height = height;
    gl_backend.resize_viewport(width, height);
}

omc::window::~window() = default;

void omc::window::update() {
    gl_backend.update_window();
    gl_backend.finish_frame();
}
#ifndef OPENMC_WINDOW_HPP
#define OPENMC_WINDOW_HPP

#include <application/opengl_backend.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace omc {
    class window {
    public:
        struct window_info {
            std::string title;
            int width;
            int height;
            bool vsync;
        };
    private:
        omc::opengl_backend gl_backend;
        omc::window::window_info info;
    public:
        explicit window(omc::window::window_info info);

        ~window();

        void update();

        void resize_callback(uint32_t width, uint32_t height);

        inline const std::string &get_title() const {
            return this->info.title;
        };

        inline void set_title(const std::string &title) {
            this->info.title = title;
            glfwSetWindowTitle(this->gl_backend.get_window(), title.c_str());
        };

        inline std::pair<int, int> get_size() const {
            return std::make_pair(this->info.width, this->info.height);
        };

        inline void set_size(int width, int height) {
            this->info.width = width;
            this->info.height = height;
            glfwSetWindowSize(this->gl_backend.get_window(), width, height);
        };

        [[nodiscard]] inline bool get_vsync() const {
            return this->info.vsync;
        };

        inline void set_vsync(bool enabled) {
            this->info.vsync = enabled;
            glfwSwapInterval(enabled ? 1 : 0);
        };

        inline GLFWwindow* get_handle() {
            return this->gl_backend.get_window();
        }

        inline omc::opengl_backend* get_opengl() {
            return &this->gl_backend;
        }

    };
}

#endif //OPENMC_WINDOW_HPP

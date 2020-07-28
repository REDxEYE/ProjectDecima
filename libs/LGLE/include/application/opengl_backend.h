//
// Created by MED45 on 14.01.2020.
//

#ifndef OPENMC_OPENGL_BACKEND_H
#define OPENMC_OPENGL_BACKEND_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/vec2.hpp>

namespace omc {
    class opengl_backend {
    public:

        opengl_backend(uint32_t initial_width, uint32_t initial_height, const std::string &app_title);

        void update_window();
        void resize_viewport(uint32_t width,uint32_t height);
        // attributes getter
        inline GLFWwindow* get_window() { return window; };
        inline bool running() { return !glfwWindowShouldClose(this->get_window()); };

        inline bool get_mouse_exclusivity() { return mouse_exclusive; };
        inline glm::vec2 get_mouse_delta() { return mouse_delta; };
        inline uint32_t get_width() { return window_width; };
        inline uint32_t get_height() { return window_height; };
        inline glm::vec2 get_mouse_pos() {
            glm::dvec2 pos(0, 0);
            glfwGetCursorPos(this->get_window(), &pos[0], &pos[1]);
            return glm::vec2(pos);
        };

        // imgui shenanigans
        static void imgui_new_frame();

        static void imgui_render_frame();



        inline void disable_mouse(){
            set_mouse_mode(GLFW_CURSOR_DISABLED);
        }
        inline void enable_mouse(){
            set_mouse_mode(GLFW_CURSOR_NORMAL);
        }
        inline void toggle_mouse(){
            mouse_exclusive=!mouse_exclusive;
            if(mouse_exclusive){
                disable_mouse();
            }else{
                enable_mouse();
            }
        }

        inline void finish_frame(){
            glfwSwapBuffers(this->get_window());
            glfwPollEvents();
        }

        static inline void clear(float r = 0, float g = 0, float b = 0, float a = 1,
                                 uint32_t flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) {
            glClear(flags);
            glClearColor(r, g, b, a);
        }

    private:

        uint32_t window_width, window_height;
        bool mouse_exclusive = true;
        glm::vec2 mouse_prev{};
        glm::vec2 mouse_delta{};

        GLFWwindow* window;

        static void init_glad();

        void init_imgui();

        inline void set_mouse_mode(uint32_t mode) {
            glfwSetInputMode(this->get_window(), GLFW_CURSOR, mode);
        }

    };

}


#endif //OPENMC_OPENGL_BACKEND_H

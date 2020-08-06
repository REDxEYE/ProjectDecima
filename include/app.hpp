//
// Created by i.getsman on 06.08.2020.
//

#ifndef PROJECTDS_APP_HPP
#define PROJECTDS_APP_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct WindowInfo {
    uint32_t m_win_width;
    uint32_t m_win_height;
    std::string m_title;
};

class App {
public:
    App(std::pair<uint32_t, uint32_t> window_size, std::string title);

    void init();

    void run();

private:
    uint64_t m_prev_time = glfwGetTime();

    void init_glfw();

    void init_opengl();

    void update(double ts);

    void begin_frame();

    void end_frame();

    void glfw_error_handler(int error, const char* message);

    void log(std::string prefix, std::string info);

protected:

    virtual void init_user();

    virtual void update_user(double ts);

    virtual void begin_frame_user();

    virtual void end_frame_user();


    inline void clear_window(float r = 0, float g = 0, float b = 0, float a = 1,
                             uint32_t flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    WindowInfo m_win_info;
    GLFWwindow* m_window;
};

#endif //PROJECTDS_APP_HPP

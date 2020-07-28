#ifndef OPENMC_APPLICATION_HPP
#define OPENMC_APPLICATION_HPP

#include <application/layer.hpp>
#include <application/window.hpp>
#include <application/opengl_backend.h>

#include <camera.h>

#include <input.h>

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace omc {
    class application : public std::enable_shared_from_this<application> {
    private:
        std::vector<std::shared_ptr<omc::layer>> layers;
        omc::input_manager input;
        omc::window window;

        double last_time = 0;


    public:


        application(const std::string &title, int width, int height, bool vsync = false);

        ~application();

        omc::opengl_backend* get_backend() { return window.get_opengl(); };

        omc::window* get_window() { return &this->window; };

        omc::input_manager* get_input() { return &this->input; };


        double get_time_delta();

        void push_layer(const std::shared_ptr<layer> &layer);

        void pop_layer();

        void run();

    };
}

#endif //OPENMC_APPLICATION_HPP

#include <application/application.hpp>

omc::application::application(const std::string &title, int width, int height, bool vsync):input(this),window({title, width, height, vsync}) {

}

omc::application::~application() {
    for (auto &layer : this->layers) {
        layer->on_detach();
    }
    glfwTerminate();
}

void omc::application::push_layer(const std::shared_ptr<omc::layer> &layer) {
    this->layers.push_back(layer);
    layer->on_attach();
}

void omc::application::pop_layer() {
    this->layers.pop_back();
}

void omc::application::run() {

    while (this->get_backend()->running()) {
        auto ts = get_time_delta();
        this->window.get_opengl()->clear();
        for (auto &layer : this->layers) {
            layer->on_update(ts);
        }

        if (input[omc::buttons::keyboard::Escape].press()) {
            get_backend()->toggle_mouse();
        }

        this->window.update();
        this->input.update();
    }
}

double omc::application::get_time_delta() {
    if (last_time == 0.f) {
        last_time = glfwGetTime();
    }
    double delta = glfwGetTime() - last_time;
    last_time = glfwGetTime();
    return delta;
}

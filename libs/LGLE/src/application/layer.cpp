//
// Created by MED45 on 18.01.2020.
//

#include <application/layer.hpp>
#include <application/application.hpp>

omc::layer::layer(omc::application* app) : app(app) {}

omc::opengl_backend* omc::layer::opengl() {
    return app->get_backend();
}

omc::camera_layer::camera_layer(omc::application* app) : layer(app) {
    camera = new omc::camera(opengl()->get_width(),
                             opengl()->get_height(), 60.0f,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec2(0.01f, 1000.0f));

}

void omc::camera_layer::on_update(double ts) {
    auto* input = app->get_input();
    if (opengl()->get_mouse_exclusivity())camera->update(opengl()->get_mouse_delta(), 0.05f);

    camera->update_viewport(opengl()->get_width(), opengl()->get_height());
    float speed = input->get(omc::buttons::keyboard::LeftShift).hold() ? 10.0f : 5.0f;

    if (input->get(omc::buttons::keyboard::W).hold()) {
        camera->move(camera->position + camera->direction * speed * float(ts));
    }

    if (input->get(omc::buttons::keyboard::S).hold()) {
        camera->move(camera->position - camera->direction * speed * float(ts));
    }

    if (input->get(omc::buttons::keyboard::A).hold()) {
        camera->move(
                camera->position - glm::normalize(glm::cross(camera->direction, {
                        0.0f, 1.0f, 0.0f
                })) * speed * float(ts));
    }

    if (input->get(omc::buttons::keyboard::D).hold()) {
        camera->move(camera->position +
                     glm::normalize(glm::cross(camera->direction, {0.0f, 1.0f, 0.0f})) * speed *
                     float(ts));
    }
}
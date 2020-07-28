#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

omc::camera::camera(int width, int height, float fov, glm::vec3 pos, glm::vec2 clip) : fov(fov), position(pos), clip(clip) {
    this->resize(width, height);
}

void omc::camera::update(glm::vec2 offset, float sensitivity) noexcept {
    offset.x *= sensitivity;
    offset.y *= sensitivity;

    this->yaw = this->yaw + offset.x;
    this->pitch = glm::clamp(this->pitch + offset.y, -89.0f, +89.0f);

    this->direction.x = glm::cos(glm::radians(this->pitch)) * glm::cos(glm::radians(this->yaw));
    this->direction.y = glm::sin(glm::radians(this->pitch));
    this->direction.z = glm::cos(glm::radians(this->pitch)) * glm::sin(glm::radians(this->yaw));
    this->direction = glm::normalize(this->direction);

    this->look(this->direction);
}

void omc::camera::resize(int w, int h) noexcept {
    this->projection = glm::perspective(glm::radians(this->fov), (float) w / (float) h, this->clip[0], this->clip[1]);
}

void omc::camera::move(glm::vec3 pos) noexcept {
    this->position = pos;
}

void omc::camera::look(glm::vec3 dir) noexcept {
    this->direction = dir;
    this->view = glm::lookAt(this->position, this->position + dir, {0.0f, 1.0f, 0.0f});
}

void omc::camera::update_viewport(uint32_t width, uint32_t height) {
    resize(width,height);
}

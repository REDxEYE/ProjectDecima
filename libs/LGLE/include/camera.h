#ifndef OPENMC_CAMERA_H
#define OPENMC_CAMERA_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>

namespace omc {
    class camera {
    public:
        glm::vec3 position {};
        glm::vec3 direction {};

        glm::mat4 projection {};
        glm::mat4 view {};

        glm::vec2 clip {};
        float pitch {};
        float yaw {};
        float fov {};

        camera(int width, int height, float fov, glm::vec3 pos, glm::vec2 clip);

        void update(glm::vec2 mouse, float sensitivity) noexcept;

        void resize(int w, int h) noexcept;

        void move(glm::vec3 pos) noexcept;

        void look(glm::vec3 dir) noexcept;

        void update_viewport(uint32_t width, uint32_t height);
    };
}

#endif //OPENMC_CAMERA_H

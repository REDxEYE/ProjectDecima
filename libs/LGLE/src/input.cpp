#include <application/application.hpp>
#include <input.h>

void omc::input_manager::update() {

    const auto update_state = [](omc::key_info &info, int new_state) {
        if (new_state == GLFW_PRESS) {
            if (info.release() || info.free()) {
                info = omc::key_info(omc::key_state::Press);
            } else if (info.press()) {
                info = omc::key_info(omc::key_state::Hold);
            }
        } else {
            if (!info.release() && !info.free()) {
                info = omc::key_info(omc::key_state::Release);
            } else if (info.release()) {
                info = omc::key_info(omc::key_state::Free);
            }
        }
    };

    for (int index = GLFW_KEY_SPACE; index < GLFW_KEY_LAST; index++) {
        update_state(this->keyboard[index],
                     glfwGetKey(app->get_window()->get_handle(), index)
        );
    }

    for (int index = GLFW_MOUSE_BUTTON_1; index < GLFW_MOUSE_BUTTON_LAST; index++) {
        update_state(this->mouse[index],
                     glfwGetMouseButton(app->get_window()->get_handle(), index)
        );
    }
}
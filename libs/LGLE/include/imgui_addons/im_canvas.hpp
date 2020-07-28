//
// Created by i.getsman on 22.07.2020.
//

#ifndef LGLE_IM_CANVAS_HPP
#define LGLE_IM_CANVAS_HPP

#include <imgui_addons/im_addons.hpp>
#include <imgui.h>
#include "object.h"
#include <memory>
#include <utility>

namespace ImGui {
    class Canvas {
        std::string name;
        float scale = 1.f;
        std::unique_ptr<omc::MemoryTexture> texture = nullptr;

        glm::vec2 _window_pos = {0.f, 0.f};
        glm::vec2 _cursor_pos = {0.f, 0.f};

    public:
        Canvas(std::string name, uint32_t width, uint32_t height, uint32_t format, uint8_t channels);

        Canvas(std::string name, omc::MemoryTexture& texture);

        inline void set_scaling(float new_scale) { scale = new_scale; };

        inline auto get_texture() { return texture.get(); };

        inline glm::vec2 mouse_pos() {
            return (glm::vec2(ImGui::GetMousePos()) -
                   (_cursor_pos + _window_pos))/scale;
        }

        inline bool mouse_over() {
            bool res = true;
            res &= (mouse_pos().x >= 0);
            res &= (mouse_pos().y >= 0);
            res &= (mouse_pos().x <= texture->width());
            res &= (mouse_pos().y <= texture->height());

            return res;
        }

        void draw();


    };
}

#endif //LGLE_IM_CANVAS_HPP

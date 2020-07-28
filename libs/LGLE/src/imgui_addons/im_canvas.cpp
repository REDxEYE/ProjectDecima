//
// Created by i.getsman on 22.07.2020.
//
#include "glm/glm.hpp"
#include "imgui_addons/im_canvas.hpp"

#include <utility>


ImGui::Canvas::Canvas(std::string name, uint32_t width, uint32_t height, uint32_t format, uint8_t channels) : name(
        std::move(name)) {
    texture = std::make_unique<omc::MemoryTexture>("CANVAS" + name, glm::vec2{width, height}, format, channels);
}

ImGui::Canvas::Canvas(std::string name, omc::MemoryTexture& texture) : name(std::move(name)), texture(&texture) {

}

void ImGui::Canvas::draw() {
    ImGui::BeginChild(("CANVAS"+name).c_str(), texture->size() * scale, false, ImGuiWindowFlags_NoMove);
    _window_pos = glm::vec2(ImGui::GetWindowPos());
    _cursor_pos = glm::vec2(ImGui::GetCursorPos());

    ImGui::Image((void*) (uintptr_t) texture->id(), texture->size() * scale);


    ImGui::EndChild();
}

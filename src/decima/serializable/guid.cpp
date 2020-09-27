#include "decima/serializable/guid.hpp"

#include <imgui.h>

void Decima::GUID::parse(ash::buffer& buffer, CoreFile& file) {
    buffer.get(m_data_1.data(), sizeof(m_data_1));
}

void Decima::GUID::draw() {
    ImGui::Text("%s", Decima::to_string(*this).c_str());
}

#include "decima/serializable/reference.hpp"
#include "decima/archive/archive_file.hpp"

#include <imgui.h>

void Decima::Ref::parse(ash::buffer& buffer, Decima::CoreFile& file) {
    m_mode = buffer.get<decltype(m_mode)>();
    if (m_mode != RefLoadMode::NotPresent)
        m_guid.parse(buffer, file);
    if (m_mode >= RefLoadMode::ImmediateCoreFile)
        m_file.parse(buffer, file);
    DECIMA_LOG("Ref::parse(", Decima::to_string(m_mode), ", ", m_file.data(), ")");
    file.queue_reference(this);
}

void Decima::Ref::draw() {
    m_guid.draw();
    ImGui::SameLine();
    ImGui::TextDisabled("Reference (%s)", Decima::to_string(m_mode).c_str());

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        m_file.draw();
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

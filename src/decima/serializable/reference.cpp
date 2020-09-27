#include "decima/serializable/reference.hpp"

#include <imgui.h>

void Decima::Ref::parse(ash::buffer& buffer, Decima::CoreFile& file) {
    m_mode = buffer.get<decltype(m_mode)>();
    if (m_mode != RefLoadMode::NotPresent)
        m_guid.parse(buffer, file);
    if (m_mode >= RefLoadMode::ImmediateCoreFile)
        m_file.parse(buffer, file);
    file.queue_reference(this);
}

void Decima::Ref::draw() {
    m_guid.draw();
    ImGui::SameLine();
    ImGui::TextDisabled("Reference (%s)", Decima::to_string(m_mode).c_str());
    ImGui::SameLine();

    if (ImGui::SmallButton("Show")) {
        m_show_object = m_object.lock() != nullptr;
    }

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

        if (m_object.lock() == nullptr) {
            ImGui::Text("Not resolved");
        } else {
            ImGui::Text("Click to show");
        }

        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    if (m_show_object) {
        ImGui::SetNextWindowSize({ 400, 200 }, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);

        if (ImGui::Begin(("Reference to " + Decima::to_string(m_guid)).c_str(), &m_show_object, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
            m_object.lock()->draw();
        }

        ImGui::End();
    }
}

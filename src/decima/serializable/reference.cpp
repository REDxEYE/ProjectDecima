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

    if (ImGui::SmallButton(("Show##" + Decima::to_string(m_guid)).c_str())) {
        m_show_object = m_object != nullptr;
    }

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

        if (m_object == nullptr) {
            ImGui::Text("Not resolved");
        } else {
            ImGui::Text("Click to show");
        }

        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    if (m_show_object) {
        ImGui::SetNextWindowSize({ 600, 400 }, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing, { 0.5, 0.5 });

        if (ImGui::Begin(("Reference to " + Decima::to_string(m_guid)).c_str(), &m_show_object, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 100);
            ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 100);

            {
                ImGui::Text("Reference");
                ImGui::NextColumn();

                ImGui::Text("%s", Decima::to_string(m_mode).c_str());
                ImGui::NextColumn();

                ImGui::Separator();
            }

            if (!m_file.data().empty()) {
                ImGui::Text("File");
                ImGui::NextColumn();

                m_file.draw();
                ImGui::NextColumn();

                ImGui::Separator();
            }

            {
                ImGui::Text("UUID");
                ImGui::NextColumn();

                m_guid.draw();
                ImGui::NextColumn();
            }

            ImGui::Columns(1);

            ImGui::Separator();

            ImGui::BeginChild(("ReferenceChild" + Decima::to_string(m_guid)).c_str());
            m_object->draw();
            ImGui::EndChild();
        }

        ImGui::End();
    }
}

//
// Created by MED45 on 07.08.2020.
//

#include "decima/file_types/translation.hpp"

#include "imgui.h"

void Decima::Translation::draw(Decima::ArchiveArray& archive_array) {
    ImGui::Columns(4);
    ImGui::SetColumnWidth(-1, 200);
    ImGui::Text("Language");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Text("Comment");
    ImGui::NextColumn();
    ImGui::Text("Flag");
    ImGui::NextColumn();

    for (std::size_t index = 0; index < std::size(Decima::Translation::languages); index++) {
        ImGui::Separator();
        ImGui::Text("%s", Decima::Translation::languages[index]);
        ImGui::NextColumn();
        ImGui::Text("%s", translations[index].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", comments[index].c_str());
        ImGui::NextColumn();
        if(flags[index]) {
            ImGui::Text("%d", flags[index]);
        } else {
            ImGui::TextDisabled("%d", flags[index]);
        }
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

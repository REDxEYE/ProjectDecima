//
// Created by MED45 on 07.08.2020.
//

#include "decima/file_types/localization.hpp"

#include "imgui.h"

void Decima::Localization::draw(Decima::ArchiveArray& archive_array) {
    if (ImGui::TreeNode(translations[0].c_str())) {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(-1, 200);
        ImGui::Text("Language");
        ImGui::NextColumn();
        ImGui::Text("Value");
        ImGui::NextColumn();

        for (std::size_t index = 0; index < Decima::Localization::languages.size(); index++) {
            ImGui::Separator();
            ImGui::Text("%s", Decima::Localization::languages[index]);
            ImGui::NextColumn();
            ImGui::Text("%s", translations[index].c_str());
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
        ImGui::TreePop();
    }
}

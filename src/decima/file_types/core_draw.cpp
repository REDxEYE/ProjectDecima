//
// Created by i.getsman on 07.08.2020.
//

#include "decima/file_types/core.h"
#include "decima/constants.hpp"
#include "imgui.h"

void Decima::CoreFile::draw(ArchiveArray& archive_array) {
    std::stringstream buffer;
    buffer << get_type_name(header.filetype) << "##" << guid;

    if (ImGui::TreeNode(buffer.str().c_str())) {
        ImGui::TextDisabled("Default handler");
        ImGui::Columns(2);
        ImGui::SetColumnWidth(-1, 200);
        ImGui::Text("Prop");
        ImGui::NextColumn();
        ImGui::Text("Value");
        ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Text("Size");
        ImGui::NextColumn();
        ImGui::Text("%i", header.file_size + 12);
        ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);
        ImGui::TreePop();
    }
}
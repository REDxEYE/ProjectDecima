//
// Created by i.getsman on 07.08.2020.
//

#include "decima/file_types/core.h"
#include "decima/constants.hpp"
#include "imgui.h"

void Decima::CoreFile::draw(ArchiveArray& archive_array) {
    ImGui::TextDisabled("Default handler");
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 200);
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();

    ImGui::Separator();

    ImGui::Text("Size");
    ImGui::NextColumn();
    ImGui::Text("%u", header.file_size + 12);
    ImGui::Separator();

    ImGui::NextColumn();
    ImGui::Text("Offset");
    ImGui::NextColumn();
    ImGui::Text("%u", offset);
    ImGui::Columns(1);
}
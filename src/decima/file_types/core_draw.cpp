//
// Created by i.getsman on 07.08.2020.
//

#include "decima/file_types/core.h"
#include "decima/constants.hpp"
#include "projectds_app.hpp"

void Decima::CoreFile::draw(ProjectDS& ctx) {
    ImGui::TextDisabled("Default handler");
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 200);
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();

    ImGui::Separator();

    ImGui::Text("Magic");
    ImGui::NextColumn();
    ImGui::Text("%s", uint64_to_hex(header.file_type).c_str());
    if (ImGui::BeginPopupContextItem("File magic")) {
        if (ImGui::Selectable("Copy file magic"))
            ImGui::SetClipboardText(uint64_to_hex(header.file_type).c_str());

        ImGui::EndPopup();
    }
    ImGui::NextColumn();

    ImGui::Separator();

    ImGui::Text("Size");
    ImGui::NextColumn();
    ImGui::Text("%u", header.file_size + 12);

    ImGui::Columns(1);
}
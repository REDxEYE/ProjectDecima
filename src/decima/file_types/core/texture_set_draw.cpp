//
// Created by MED45 on 11.08.2020.
//
#include <sstream>

#include "decima/file_types/core/texture_set.h"
#include "imgui.h"

void Decima::TextureSet::draw() {
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 200);
    ImGui::Text("Prop");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Unk entries");
    ImGui::NextColumn();

    ImGui::BeginChild("Unk entries##child",{0,200});
    ImGui::Columns(1);
    for (auto& entry : file_entries) {
        std::stringstream ss;
        ss << entry.guid;
        ImGui::Text("%s", ss.str().c_str());
        ImGui::Separator();
    }
    ImGui::EndChild();
    ImGui::NextColumn();


    ImGui::Text("Src entries");
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::BeginChild("Src entries##child",{0,400});

    for (auto& entry : src_entries) {
        ImGui::Text("Slot");
        ImGui::NextColumn();
        ImGui::Text("%i",entry.slot_id);
        ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Text("Src name");
        ImGui::NextColumn();
        entry.src_name.draw();
        ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Text("Unk floats");
        ImGui::NextColumn();
        ImGui::DragFloat4("##",entry.unk3);
        ImGui::NextColumn();
        ImGui::Separator();

        ImGui::NextColumn();
        ImGui::Spacing();
        ImGui::NextColumn();
        ImGui::Spacing();
        ImGui::NextColumn();
        ImGui::Spacing();
        ImGui::NextColumn();
        ImGui::Spacing();
//        ImGui::NextColumn();
    }
    ImGui::EndChild();

}
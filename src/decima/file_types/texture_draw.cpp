//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/texture.h"
#include "decima/archive/archive_array.h"
#include "utils.h"

#include "imgui.h"

void Decima::Texture::draw(ProjectDS& ctx) {
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 200);
    ImGui::Text("Prop");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Unk1");
    ImGui::NextColumn();
    ImGui::Text("%i", unk1);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Width");
    ImGui::NextColumn();
    ImGui::Text("%i", width);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Height");
    ImGui::NextColumn();
    ImGui::Text("%i", height);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("layers");
    ImGui::NextColumn();
    ImGui::Text("%i", layers);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Mip count");
    ImGui::NextColumn();
    ImGui::Text("%i", mip_count);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Pixel format");
    ImGui::NextColumn();
    {
        std::stringstream buffer;
        buffer << pixel_format;
        ImGui::Text("%s", buffer.str().c_str());
    }
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unk2");
    ImGui::NextColumn();
    ImGui::Text("%i", unk2);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unk3");
    ImGui::NextColumn();
    ImGui::Text("%i", unk3);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("GUID");
    ImGui::NextColumn();
    {
        std::stringstream buffer;
        buffer << file_guid;
        ImGui::Text("%s", buffer.str().c_str());
    }
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("buffer size");
    ImGui::NextColumn();
    ImGui::Text("%i", buffer_size);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("total size");
    ImGui::NextColumn();
    ImGui::Text("%i", total_size);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unks-0");
    ImGui::NextColumn();
    ImGui::Text("%i", unks[0]);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unks-1");
    ImGui::NextColumn();
    ImGui::Text("%i", unks[1]);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unks-2");
    ImGui::NextColumn();
    ImGui::Text("%i", unks[2]);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unks-3");
    ImGui::NextColumn();
    ImGui::Text("%i", unks[3]);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Stream");
    ImGui::NextColumn();
    ImGui::Text("%s", stream_name.c_str());
    if (ImGui::BeginPopupContextItem("Stream name")) {
        if (ImGui::Selectable("Copy stream path"))
            ImGui::SetClipboardText((stream_name+".core.stream").c_str());
        ImGui::EndPopup();
    }
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Columns(1);
}

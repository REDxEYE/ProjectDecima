//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/core/texture.h"
#include "utils.h"
#include "projectds_app.hpp"

#include <detex.h>
#include <detex-png.h>
#include <portable-file-dialogs.h>

void Decima::Texture::draw() {
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
    ImGui::Text("%i", total_mips);
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

    ImGui::Text("Stream Size");
    ImGui::NextColumn();
    ImGui::Text("%i", stream_size);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Stream Mips");
    ImGui::NextColumn();
    ImGui::Text("%i", stream_mips);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unk4");
    ImGui::NextColumn();
    ImGui::Text("%i", unk4);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unk5");
    ImGui::NextColumn();
    ImGui::Text("%i", unk5);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Stream");
    ImGui::NextColumn();
    if (stream_size > 0) {
        ImGui::BeginChild(file_guid.hash(), { 0, 150 }, true);
        external_data.draw();
        ImGui::EndChild();
    } else {
        ImGui::Text("No external stream");
    }

    draw_texture(256, 256, 128, 4);

    ImGui::NextColumn();
    ImGui::Columns(1);
}

void Decima::Texture::draw_texture(float preview_width, float preview_height, float zoom_region, float zoom_scale) {
    if(image_mips.size() > 1) {
        if (ImGui::ArrowButton("Up", ImGuiDir_Left))
            mip_index = std::max(0, mip_index - 1);
        ImGui::SameLine();
        if (ImGui::ArrowButton("Down", ImGuiDir_Right))
            mip_index = std::min(int(image_mips.size()) - 1, mip_index + 1);
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        ImGui::DragInt("##", &mip_index, 0.05f, 0, image_mips.size() - 1, "Mip #%d");
    }

    ImGui::Text("Mip #%d (%dx%d)", mip_index, width >> mip_index, height >> mip_index);

    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const ImVec4 tint = { 1, 1, 1, 1 };
    const ImVec4 border = { 1, 1, 1, 1 };
    ImGui::Image(reinterpret_cast<ImTextureID>(image_mips[mip_index].first), { preview_width, preview_height }, { 0, 0 }, { 1, 1 }, tint, border);

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();

        auto& io = ImGui::GetIO();
        auto region_x = io.MousePos.x - pos.x - zoom_region * 0.5f;
        auto region_y = io.MousePos.y - pos.y - zoom_region * 0.5f;

        if (region_x < 0.0f) {
            region_x = 0.0f;
        } else if (region_x > preview_width - zoom_region) {
            region_x = preview_width - zoom_region;
        }

        if (region_y < 0.0f) {
            region_y = 0.0f;
        } else if (region_y > preview_height - zoom_region) {
            region_y = preview_height - zoom_region;
        }

        ImVec2 uv0 = { region_x / preview_width, region_y / preview_height };
        ImVec2 uv1 = { (region_x + zoom_region) / preview_width, (region_y + zoom_region) / preview_height };
        ImGui::Image(reinterpret_cast<ImTextureID>(image_mips[mip_index].first), ImVec2(zoom_region * zoom_scale, zoom_region * zoom_scale), uv0, uv1, tint, border);

        ImGui::EndTooltip();
    }
}
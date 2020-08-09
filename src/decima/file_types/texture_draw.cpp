//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/texture.h"
#include "decima/archive/archive_array.h"
#include "utils.h"
#include "projectds_app.hpp"

#include <detex.h>
#include <imgui.h>

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

    ImGui::Text("Stream Size");
    ImGui::NextColumn();
    ImGui::Text("%i", stream_size);
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("unks-1");
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

    ImGui::Text("Stream");
    ImGui::NextColumn();
    ImGui::Text("%s", stream_name.c_str());
    if (ImGui::BeginPopupContextItem("Stream name")) {
        if (ImGui::Selectable("Copy stream path"))
            ImGui::SetClipboardText((stream_name + ".core.stream").c_str());
        ImGui::EndPopup();
    }
    draw_texture(ctx, 128, 128);
    ImGui::NextColumn();
    ImGui::Columns(1);
}

static bool decompress_texture(std::vector<std::uint8_t>& src, std::vector<std::uint8_t>& dst, int width, int height, int fmt) {
    detexTexture texture;
    texture.format = fmt;
    texture.data = src.data();
    texture.width = width;
    texture.height = height;
    texture.width_in_blocks = int(width / (detexGetCompressedBlockSize(fmt) / 2));
    texture.height_in_blocks = int(height / (detexGetCompressedBlockSize(fmt) / 2));

    if (!detexDecompressTextureLinear(&texture, dst.data(), DETEX_PIXEL_FORMAT_RGBA8)) {
        std::printf("Buffer cannot be decompressed: %s\n", detexGetErrorMessage());
        return false;
    }

    return true;
}

void Decima::Texture::draw_texture(ProjectDS& ctx, float preview_width, float preview_height) {
    static const std::unordered_map<TexturePixelFormat, int> format_mapper = {
        { TexturePixelFormat::BC1, DETEX_TEXTURE_FORMAT_BC1 },
        { TexturePixelFormat::BC2, DETEX_TEXTURE_FORMAT_BC2 },
        { TexturePixelFormat::BC3, DETEX_TEXTURE_FORMAT_BC3 }
    };

    if (stream_size > 0 && stream_buffer.empty()) {
        const auto format = format_mapper.find(pixel_format);

        if (format == format_mapper.end()) {
            std::stringstream buffer;
            buffer << "Image pixel format is not supported: " << pixel_format;

            ImGui::Text("%s", buffer.str().c_str());
            return;
        }

        auto stream_file = ctx.archive_array.query_file(stream_name + ".core.stream");
        stream_file.unpack(0);

        stream_buffer = std::move(stream_file.storage);
        image_buffer.resize(width * height * 4);

        if (!decompress_texture(stream_buffer, image_buffer, width, height, format->second)) {
            std::puts("Texture was failed to decode");
            return;
        }

        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer.data());
    }

    ImGui::Image(reinterpret_cast<ImTextureID>(image_texture), { preview_width, preview_height }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 });
}

Decima::Texture::~Texture() {
    if (image_texture > 0)
        glDeleteTextures(1, &image_texture);
}
#include "decima/serializable/object/texture.hpp"

#include <util/pfd.h>
#include <fstream>

#include "utils.hpp"
#include "projectds_app.hpp"

void Decima::Texture::draw() {
    ImGui::Columns(2);

    {
        ImGui::Text("Property");
        ImGui::NextColumn();

        ImGui::Text("Value");
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::SetColumnWidth(0, 200);
    ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 200);

    {
        ImGui::Text("Type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Width");
        ImGui::NextColumn();

        ImGui::Text("%u", width);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Height");
        ImGui::NextColumn();

        ImGui::Text("%u", height);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Layers");
        ImGui::NextColumn();

        ImGui::Text("%u", layers);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Total mip-maps");
        ImGui::NextColumn();

        ImGui::Text("%u", total_mips);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Format");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(pixel_format).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Unknown #0");
        ImGui::NextColumn();

        ImGui::Text("%u", unk_0);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Unknown #1");
        ImGui::NextColumn();

        ImGui::Text("%u", unk_1);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Unknown #2");
        ImGui::NextColumn();

        guid.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Buffer size");
        ImGui::NextColumn();

        ImGui::Text("%u", buffer_size);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Total size");
        ImGui::NextColumn();

        ImGui::Text("%u", total_size);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Stream size");
        ImGui::NextColumn();

        ImGui::Text("%u", stream_size);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Stream mip-maps");
        ImGui::NextColumn();

        ImGui::Text("%u", stream_mips);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Unknown #3");
        ImGui::NextColumn();

        ImGui::Text("%u", unk_3);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Unknown #4");
        ImGui::NextColumn();

        ImGui::Text("%u", unk_4);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Stream");
        ImGui::NextColumn();

        if (stream_size > 0) {
            ImGui::BeginChild(unk_2.hash(), { 0, 150 }, true);
            external_data.draw();
            ImGui::EndChild();
        } else {
            ImGui::Text("No external stream");
        }

        draw_preview(256, 256, 128, 4);

        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

void Decima::Texture::draw_preview(float preview_width, float preview_height, float zoom_region, float zoom_scale) {
    if (mip_textures.empty()) {
        ImGui::TextDisabled("No preview available");
        return;
    }

    if (mip_textures.size() > 1) {
        if (ImGui::ArrowButton("Up", ImGuiDir_Left))
            mip_index = std::max(0, mip_index - 1);
        ImGui::SameLine();
        if (ImGui::ArrowButton("Down", ImGuiDir_Right))
            mip_index = std::min(int(mip_textures.size()) - 1, mip_index + 1);
        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        ImGui::DragInt("##", &mip_index, 0.05f, 0, mip_textures.size() - 1, "Mip #%d");
    }

    ImGui::Text("Mip #%d (%s, %dx%d)", mip_index, mip_index >= stream_mips ? "Internal" : "External", width >> mip_index, height >> mip_index);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const ImVec4 tint = { 1, 1, 1, 1 };
    const ImVec4 border = { 1, 1, 1, 1 };
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<std::size_t>(mip_textures[mip_index])), { preview_width, preview_height }, { 0, 0 }, { 1, 1 }, tint, border);

    if (ImGui::BeginPopupContextItem("Export Image")) {
        if (ImGui::Selectable("Export image")) {
            auto full_path = pfd::save_file("Choose destination file", "", { "Truevision TGA (TARGA)", "*.tga" }).result();

            if (!full_path.empty()) {
                full_path += ".tga";

                DECIMA_PACK(struct TGAHeader {
                    std::uint8_t id_length;
                    std::uint8_t color_map_type;
                    std::uint8_t image_type;
                    std::uint16_t color_map_origin;
                    std::uint16_t color_map_length;
                    std::uint8_t color_map_depth;
                    std::uint16_t x_origin;
                    std::uint16_t y_origin;
                    std::uint16_t width;
                    std::uint16_t height;
                    std::uint8_t pixel_depth;
                    std::uint8_t image_descriptor;
                });

                TGAHeader tga_header { 0 };
                tga_header.image_type = 2;
                tga_header.width = width;
                tga_header.height = height;
                tga_header.pixel_depth = 32;
                tga_header.image_descriptor = 8;

                DECIMA_PACK(struct TGAFooter {
                    std::uint32_t extension_offset;
                    std::uint32_t dev_area_offset;
                    std::int8_t signature[18];
                });

                TGAFooter tga_footer { 0 };
                std::memcpy(tga_footer.signature, "TRUEVISION-XFILE.", 18);

                std::vector<std::uint32_t> buffer;
                buffer.resize(width * height);

                glGetTextureImage(mip_textures[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.size() * 4, buffer.data());

                for (std::size_t y = 0; y < height / 2; y++) {
                    for (std::size_t x = 0; x < width; x++) {
                        auto& a = buffer[y * width + x];
                        auto& b = buffer[(height - y - 1) * width + x];

                        constexpr auto rgb2bgr = [](std::uint32_t color) {
                            return (color & 0xff000000) | ((color & 0xff0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16);
                        };

                        a = rgb2bgr(a);
                        b = rgb2bgr(b);

                        std::swap(a, b);
                    }
                }

                std::ofstream writer { full_path, std::ios::binary | std::ios::trunc };
                writer.write(reinterpret_cast<const char*>(&tga_header), sizeof(tga_header));
                writer.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * 4);
                writer.write(reinterpret_cast<const char*>(&tga_footer), sizeof(tga_footer));

                DECIMA_LOG("File was saved to: ", full_path);
            }
        }

        ImGui::EndPopup();
    }

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
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<std::size_t>(mip_textures[mip_index])), ImVec2(zoom_region * zoom_scale, zoom_region * zoom_scale), uv0, uv1, tint, border);

        ImGui::EndTooltip();
    }
}

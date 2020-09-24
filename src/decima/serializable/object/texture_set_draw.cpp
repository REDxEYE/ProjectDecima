#include "decima/serializable/object/texture_set.hpp"

#include <imgui.h>

void Decima::TextureDefaultColor::draw() {
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputFloat4("##", rgba, "%.3f", ImGuiInputTextFlags_ReadOnly);
}

void Decima::DecimaTextureSetEntry::draw() {
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
        ImGui::Text("Compression method");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(compression_method).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Create mip-maps");
        ImGui::NextColumn();

        ImGui::Text("%u", create_mip_maps);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Color space");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(color_space).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Packing info");
        ImGui::NextColumn();

        ImGui::Text("%u", packing_info);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Texture type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(texture_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Texture");
        ImGui::NextColumn();

        texture.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::Columns(1);
}

void Decima::DecimaTextureSetTextureDescriptor::draw() {
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
        ImGui::Text("Texture type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(texture_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Path");
        ImGui::NextColumn();

        path.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Active");
        ImGui::NextColumn();

        ImGui::Text("%u", active);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Gamma space");
        ImGui::NextColumn();

        ImGui::Text("%u", gamma_space);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Storage type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(storage_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Quality type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(quality_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Compression method");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(compression_method).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    if(active > 0) {
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
    } else {
        {
            ImGui::Text("Unknown #0");
            ImGui::NextColumn();

            ImGui::Text("%u", unk_0);
            ImGui::NextColumn();

            ImGui::Separator();
        }
    }

    {
        ImGui::Text("Default color");
        ImGui::NextColumn();

        default_color.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::Columns(1);
}

void Decima::TextureSet::draw() {
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
        ImGui::Text("Mip-map mode");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(mip_map_mode).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Preset");
        ImGui::NextColumn();

        preset.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::Columns(1);

    if (ImGui::TreeNodeEx("Entries", ImGuiTreeNodeFlags_SpanFullWidth)) {
        for (std::size_t entry_index = 0; entry_index < entries.size(); entry_index++) {
            const auto entry_name = "Entry #" + std::to_string(entry_index);

            if (ImGui::TreeNodeEx(entry_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                entries.at(entry_index).draw();
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Descriptors", ImGuiTreeNodeFlags_SpanFullWidth)) {
        for (std::size_t descriptor_index = 0; descriptor_index < descriptors.size(); descriptor_index++) {
            const auto descriptor_name = "Descriptor #" + std::to_string(descriptor_index);

            if (ImGui::TreeNodeEx(descriptor_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                descriptors.at(descriptor_index).draw();
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}

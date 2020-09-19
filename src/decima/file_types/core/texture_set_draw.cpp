#include "decima/file_types/core/texture_set.h"

#include <imgui.h>

template <>
std::string Decima::to_string(const Decima::TextureColorSpace& value) {
    switch (value) {
    case TextureColorSpace::Linear:
        return "Linear";
    case TextureColorSpace::SRGB:
        return "sRGB";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
std::string Decima::to_string(const Decima::TextureSetType& value) {
    switch (value) {
    case TextureSetType::Invalid:
        return "Invalid";
    case TextureSetType::Color:
        return "Color";
    case TextureSetType::Alpha:
        return "Alpha";
    case TextureSetType::Normal:
        return "Normal";
    case TextureSetType::Reflectance:
        return "Reflectance";
    case TextureSetType::AO:
        return "AO";
    case TextureSetType::Roughness:
        return "Roughness";
    case TextureSetType::Height:
        return "Height";
    case TextureSetType::Mask:
        return "Mask";
    case TextureSetType::MaskAlpha:
        return "Mask alpha";
    case TextureSetType::Incandescence:
        return "Incandescence";
    case TextureSetType::TranslucencyDiffusion:
        return "Translucency diffusion";
    case TextureSetType::TranslucencyAmount:
        return "Translucency amount";
    case TextureSetType::Misc_01:
        return "Misc_01";
    case TextureSetType::Curvature:
        return "Curvature";
    case TextureSetType::Luminance:
        return "Luminance";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
std::string Decima::to_string(const Decima::TextureSetStorageType& value) {
    switch (value) {
    case TextureSetStorageType::RGB:
        return "RGB";
    case TextureSetStorageType::R:
        return "R";
    case TextureSetStorageType::G:
        return "G";
    case TextureSetStorageType::B:
        return "B";
    case TextureSetStorageType::A:
        return "A";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
std::string Decima::to_string(const Decima::TextureSetQualityType& value) {
    switch (value) {
    case TextureSetQualityType::Default:
        return "Default";
    case TextureSetQualityType::CompressedHigh:
        return "Compressed (High)";
    case TextureSetQualityType::CompressedLow:
        return "Compressed (Low)";
    case TextureSetQualityType::Uncompressed:
        return "Uncompressed";
    case TextureSetQualityType::NormalBC6:
        return "Normal (BC6)";
    case TextureSetQualityType::NormalHigh:
        return "Normal (High)";
    case TextureSetQualityType::NormalLow:
        return "Normal (Low)";
    case TextureSetQualityType::BC4:
        return "BC4";
    case TextureSetQualityType::Clean:
        return "Clean";
    case TextureSetQualityType::NormalRoughnessBC7:
        return "Normal roughness (BC7)";
    case TextureSetQualityType::AlphaToCoverageBC4:
        return "Alpha to coverage (BC4)";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
std::string Decima::to_string(const Decima::TextureCompressionMethod& value) {
    switch (value) {
    case TextureCompressionMethod::PerceptualData:
        return "Perceptual data";
    case TextureCompressionMethod::NormalData:
        return "Normal data";
    case TextureCompressionMethod::VariableData:
        return "Variable data";
    case TextureCompressionMethod::DefaultData:
        return "Default data";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
std::string Decima::to_string(const Decima::DecimaTextureMipMapMode& value) {
    switch (value) {
    case DecimaTextureMipMapMode::Wrap:
        return "Wrap";
    case DecimaTextureMipMapMode::Clamp:
        return "Clamp";
    case DecimaTextureMipMapMode::Mirror:
        return "Mirror";
    case DecimaTextureMipMapMode::ClampToBorder:
        return "Clamp to border";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

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

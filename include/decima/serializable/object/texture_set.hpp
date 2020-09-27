#pragma once

#include <vector>

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/string.hpp"
#include "decima/serializable/guid.hpp"
#include "decima/serializable/reference.hpp"

namespace Decima {
    enum class TextureColorSpace : std::uint32_t {
        Linear = 0,
        SRGB = 1
    };

    enum class TextureSetType : std::uint32_t {
        Invalid = 0,
        Color = 1,
        Alpha = 2,
        Normal = 3,
        Reflectance = 4,
        AO = 5,
        Roughness = 6,
        Height = 7,
        Mask = 8,
        MaskAlpha = 9,
        Incandescence = 10,
        TranslucencyDiffusion = 11,
        TranslucencyAmount = 12,
        Misc_01 = 13,
        Curvature = 14,
        Luminance = 15
    };

    enum class TextureSetStorageType : std::uint32_t {
        RGB = 0,
        R = 1,
        G = 2,
        B = 3,
        A = 4
    };

    enum class TextureSetQualityType : std::uint32_t {
        Default = 0,
        CompressedHigh = 1,
        CompressedLow = 2,
        Uncompressed = 3,
        NormalBC6 = 4,
        NormalHigh = 5,
        NormalLow = 6,
        BC4 = 8,
        Clean = 7,
        NormalRoughnessBC7 = 9,
        AlphaToCoverageBC4 = 10
    };

    enum class TextureCompressionMethod : std::uint32_t {
        PerceptualData = 0,
        NormalData = 1,
        VariableData = 2,
        DefaultData = 3
    };

    enum class DecimaTextureMipMapMode : std::uint32_t {
        Wrap = 0,
        Clamp = 1,
        Mirror = 2,
        ClampToBorder = 3
    };

    class TextureDefaultColor {
    public:
        void parse(ash::buffer& buffer, CoreFile& file);
        void draw();

    private:
        float rgba[4];
    };

    class DecimaTextureSetEntry {
    public:
        void parse(ash::buffer& buffer, CoreFile& file);
        void draw();

    private:
        TextureCompressionMethod compression_method;
        std::uint8_t create_mip_maps;
        TextureColorSpace color_space;
        std::uint32_t packing_info;
        TextureSetType texture_type;
        Ref texture;
    };

    struct DecimaTextureSetTextureDescriptor {
    public:
        void parse(ash::buffer& buffer, CoreFile& file);
        void draw();

    private:
        TextureSetType texture_type;
        StringHashed path;
        std::uint8_t active;
        std::uint8_t gamma_space;
        TextureSetStorageType storage_type;
        TextureSetQualityType quality_type;
        TextureCompressionMethod compression_method;
        std::uint32_t width {};
        std::uint32_t height {};
        std::uint32_t unk_0 {};
        TextureDefaultColor default_color;
    };

    class TextureSet : public CoreObject {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) override;
        void draw() override;

    private:
        std::vector<DecimaTextureSetEntry> entries;
        DecimaTextureMipMapMode mip_map_mode;
        std::vector<DecimaTextureSetTextureDescriptor> descriptors;
        Ref preset;
    };
}

template <>
inline std::string Decima::to_string(const Decima::TextureColorSpace& value) {
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
inline std::string Decima::to_string(const Decima::TextureSetType& value) {
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
inline std::string Decima::to_string(const Decima::TextureSetStorageType& value) {
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
inline std::string Decima::to_string(const Decima::TextureSetQualityType& value) {
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
inline std::string Decima::to_string(const Decima::TextureCompressionMethod& value) {
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
inline std::string Decima::to_string(const Decima::DecimaTextureMipMapMode& value) {
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

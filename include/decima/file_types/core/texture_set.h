#pragma once

#include <vector>

#include "decima/core_file.hpp"
#include "decima/file_types/core/entry.hpp"
#include "decima/file_types/pod/string.hpp"
#include "decima/file_types/pod/guid.hpp"

namespace Decima {
    enum class TextureColorSpace : std::uint32_t {
        Linear = 0,
        SRGB = 1
    };

    std::ostream& operator<<(std::ostream& os, Decima::TextureColorSpace);

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

    std::ostream& operator<<(std::ostream& os, Decima::TextureSetType);

    enum class TextureSetStorageType : std::uint32_t {
        RGB = 0,
        R = 1,
        G = 2,
        B = 3,
        A = 4
    };

    std::ostream& operator<<(std::ostream& os, Decima::TextureSetStorageType);

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

    std::ostream& operator<<(std::ostream& os, Decima::TextureSetQualityType);

    enum class TextureCompressionMethod : std::uint32_t {
        PerceptualData = 0,
        NormalData = 1,
        VariableData = 2,
        DefaultData = 3
    };

    std::ostream& operator<<(std::ostream& os, Decima::TextureCompressionMethod);

    enum class DecimaTextureMipMapMode : std::uint32_t {
        Wrap = 0,
        Clamp = 1,
        Mirror = 2,
        ClampToBorder = 3
    };

    std::ostream& operator<<(std::ostream& os, Decima::DecimaTextureMipMapMode);

    class TextureDefaultColor {
    public:
        float rgba[4];

        void parse(ash::buffer& buffer);
        void draw();
    };

    class DecimaTextureSetEntry {
    public:
        TextureCompressionMethod compression_method;
        std::uint8_t create_mip_maps;
        TextureColorSpace color_space;
        std::uint32_t packing_info;
        TextureSetType texture_type;
        Reference texture;

        void parse(ash::buffer& buffer);
        void draw();
    };

    struct DecimaTextureSetTextureDescriptor {
        TextureSetType texture_type;
        StringHashed path;
        std::uint8_t active;
        std::uint8_t gamma_space;
        TextureSetStorageType storage_type;
        TextureSetQualityType quality_type;
        TextureCompressionMethod compression_method;
        std::uint32_t width {};
        std::uint32_t height {};
        TextureDefaultColor default_color;

        void parse(ash::buffer& buffer);
        void draw();
    };

    class TextureSet : public CoreEntry {
        std::vector<DecimaTextureSetEntry> entries;
        DecimaTextureMipMapMode mip_map_mode;
        std::vector<DecimaTextureSetTextureDescriptor> descriptors;
        Reference preset;

    public:
        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;
    };
}

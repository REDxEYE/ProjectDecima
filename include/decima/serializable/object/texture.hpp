#pragma once

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/stream.hpp"

#include <glad/glad.h>

namespace Decima {
    enum class TexturePixelFormat : std::uint8_t {
        RGBA8 = 0xC,
        RGBA16F = 0x13,
        A8 = 0x1F,
        BC1 = 0x42,
        BC2 = 0x43,
        BC3 = 0x44,
        BC4 = 0x45,
        BC5 = 0x47,
        BC6 = 0x49,
        BC7 = 0x4B
    };

    enum class TextureType : std::uint16_t {
        Tex2D = 0,
        Tex3D = 1,
        TexCubeMap = 2,
        Tex2DArray = 3
    };

    struct TexturePixelFormatInfo {
        /* This format's block size (in pixels) */
        int block_size;
        /* How many bits occupies one pixel */
        int block_density;
        /* Corresponding OpenGL internal format */
        int internal_format;
        /* Texture format */
        int data_format;
        /* Is format whether compressed or not */
        bool compressed;

        unsigned int calculate_size(int width, int height) const;
        unsigned int create_texture(int width, int height, const void* data, std::size_t size) const;
    };

    extern const std::unordered_map<TexturePixelFormat, TexturePixelFormatInfo> texture_format_info;

    class Texture : public CoreObject {
    public:
        ~Texture();

        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;

    private:
        void draw_preview(float preview_width, float preview_height, float zoom_region, float zoom_scale);

        TextureType type;
        std::uint16_t width;
        std::uint16_t height;
        std::uint16_t layers;
        std::uint8_t total_mips;
        TexturePixelFormat pixel_format;
        std::uint16_t unk_0;
        std::uint32_t unk_1;
        GUID unk_2;
        std::uint32_t buffer_size;
        std::uint32_t total_size;
        std::uint32_t stream_size;
        std::uint32_t stream_mips;
        std::uint32_t unk_3;
        std::uint32_t unk_4;
        Decima::Stream external_data;
        std::vector<char> embedded_data;
        std::vector<unsigned int> mip_textures;
        int mip_index;
    };
}

template <>
inline std::string Decima::to_string(const TexturePixelFormat& value) {
    switch (value) {
    case Decima::TexturePixelFormat::RGBA8:
        return "RGBA8";
    case Decima::TexturePixelFormat::RGBA16F:
        return "RGBA16F";
    case Decima::TexturePixelFormat::A8:
        return "A8";
    case Decima::TexturePixelFormat::BC1:
        return "BC1";
    case Decima::TexturePixelFormat::BC2:
        return "BC2";
    case Decima::TexturePixelFormat::BC3:
        return "BC3";
    case Decima::TexturePixelFormat::BC4:
        return "BC4";
    case Decima::TexturePixelFormat::BC5:
        return "BC5";
    case Decima::TexturePixelFormat::BC6:
        return "BC6";
    case Decima::TexturePixelFormat::BC7:
        return "BC7";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

template <>
inline std::string Decima::to_string(const TextureType& value) {
    switch (value) {
    case TextureType::Tex2D:
        return "2D";
    case TextureType::Tex3D:
        return "3D";
    case TextureType::TexCubeMap:
        return "CubeMap";
    case TextureType::Tex2DArray:
        return "2D Array";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}

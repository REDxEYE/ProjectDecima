#pragma once

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#include "decima/file_types/pod/stream.hpp"
#include "decima/file_types/core/entry.hpp"

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

    class Texture : public CoreEntry {
    public:
        ~Texture();

        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
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

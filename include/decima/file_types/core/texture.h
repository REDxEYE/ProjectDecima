//
// Created by MED45 on 07.08.2020.
//

#ifndef PROJECTDS_TEXTURE_H
#define PROJECTDS_TEXTURE_H

#include "decima/file_types/pod/stream.hpp"
#include "decima/file_types/core/core.h"

namespace Decima {
    enum class TexturePixelFormat : uint8_t {
        RGBA8 = 0xC,
        A8 = 0x1F,
        BC1 = 0x42,
        BC2 = 0x43,
        BC3 = 0x44,
        BC4 = 0x45,
        BC5 = 0x47,
        BC7 = 0x4B,
    };

    std::ostream& operator<<(std::ostream& os, Decima::TexturePixelFormat fmt);

    class Texture : public CoreFile {
    public:
        ~Texture();

        std::uint16_t unk1 {};
        std::uint16_t width {};
        std::uint16_t height {};
        std::uint16_t layers {};
        std::uint8_t total_mips {};
        TexturePixelFormat pixel_format {};
        std::uint16_t unk2 {};
        std::uint32_t unk3 {};
        GUID file_guid {};
        std::uint32_t buffer_size {};
        std::uint32_t total_size {};
        std::uint32_t stream_size {};
        std::uint32_t stream_mips {};
        std::uint32_t unk4 {};
        std::uint32_t unk5 {};

        Decima::Stream external_data;
        std::vector<std::pair<unsigned int, std::vector<std::uint8_t>>> image_mips;
        int mip_index{};

        void parse(ArchiveArray& archives, Source& stream) override;
        void draw() override;

    private:
        void draw_texture(float preview_width, float preview_height, float zoom_region, float zoom_scale);
    };
}
#endif //PROJECTDS_TEXTURE_H
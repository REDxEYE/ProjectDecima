//
// Created by MED45 on 07.08.2020.
//

#ifndef PROJECTDS_TEXTURE_H
#define PROJECTDS_TEXTURE_H

#include "decima/file_types/pod/stream.hpp"
#include "decima/file_types/core/entry.h"

namespace Decima {
    enum class TexturePixelFormat : uint8_t {
        RGBA8 = 0xC,
        RGBA16F = 0x13,
        A8 = 0x1F,
        BC1 = 0x42,
        BC2 = 0x43,
        BC3 = 0x44,
        BC4 = 0x45,
        BC5 = 0x47,
        BC6 = 0x49,
        BC7 = 0x4B,
    };

    std::ostream& operator<<(std::ostream& os, Decima::TexturePixelFormat fmt);

    enum class TextureFlags : std::uint16_t {
        Cubemap = 0x02
    };

    std::ostream& operator<<(std::ostream& os, Decima::TextureFlags fmt);

    class Texture : public CoreEntry {
    public:
        ~Texture();

        TextureFlags flags {};
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
        std::vector<std::uint8_t> embedded_data;
        std::vector<unsigned int> mip_textures;
        int mip_index;

        void parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) override;
        void draw() override;

    private:
        void draw_preview(float preview_width, float preview_height, float zoom_region, float zoom_scale);
    };
}
#endif //PROJECTDS_TEXTURE_H

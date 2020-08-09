//
// Created by MED45 on 07.08.2020.
//

#ifndef PROJECTDS_TEXTURE_H
#define PROJECTDS_TEXTURE_H

#include "decima/file_types/core.h"

namespace Decima {
    enum class TexturePixelFormat : uint8_t {
        R8G8B8A8 = 0xC,
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
        uint16_t unk1 {};
        uint16_t width {};
        uint16_t height {};
        uint16_t layers {};
        uint8_t mip_count {};
        TexturePixelFormat pixel_format {};
        uint16_t unk2 {};
        uint32_t unk3 {};
        GUID file_guid {};
        uint32_t buffer_size {};
        uint32_t total_size {};
        uint32_t unks[4] {};

        std::string stream_name = "NO_EXTERNAL_STREAM";

        void parse(Source& stream) override;
        void draw(ProjectDS& ctx) override;
    };
}
#endif //PROJECTDS_TEXTURE_H

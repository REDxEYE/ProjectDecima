//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/core/texture.h"

#include <glad/glad.h>
#include <detex.h>

/*
 * <enum: int> <pixel_bits: int>
 */
using DXGIPixelFormat = std::pair<int, int>;

/*
 * This is very clunky approach. Maybe rewrite into something
 * similar to what I did in ProjectDS::parse_core_file using
 * factory pattern
 */
static const std::unordered_map<Decima::TexturePixelFormat, DXGIPixelFormat> format_mapper = {
    { Decima::TexturePixelFormat::BC1, { DETEX_TEXTURE_FORMAT_BC1, 8 } },
    { Decima::TexturePixelFormat::BC2, { DETEX_TEXTURE_FORMAT_BC2, 16 } },
    { Decima::TexturePixelFormat::BC3, { DETEX_TEXTURE_FORMAT_BC3, 16 } },
    { Decima::TexturePixelFormat::BC4, { DETEX_TEXTURE_FORMAT_RGTC1, 8 } },
    { Decima::TexturePixelFormat::BC5, { DETEX_TEXTURE_FORMAT_RGTC2, 16 } },
    { Decima::TexturePixelFormat::BC7, { DETEX_TEXTURE_FORMAT_BPTC, 16 } },
    { Decima::TexturePixelFormat::RGBA8, { DETEX_TEXTURE_FORMAT_BPTC, 16 } },
};

static bool decompress_texture(const std::uint8_t* src, std::vector<std::uint8_t>& dst, int width, int height, int fmt);

void Decima::Texture::parse(ArchiveArray& archives, Source& stream) {
    CoreFile::parse(archives, stream);
    uint64_t start = stream.tell();

    unk1 = stream.read<typeof(unk1)>();
    width = stream.read<typeof(width)>();
    height = stream.read<typeof(height)>();
    layers = stream.read<typeof(layers)>();
    total_mips = stream.read<typeof(total_mips)>();
    pixel_format = stream.read<typeof(pixel_format)>();
    unk2 = stream.read<typeof(unk2)>();
    unk3 = stream.read<typeof(unk3)>();
    file_guid.parse(stream);
    buffer_size = stream.read<typeof(buffer_size)>();
    total_size = stream.read<typeof(total_size)>();
    stream_size = stream.read<typeof(stream_size)>();
    stream_mips = stream.read<typeof(stream_mips)>();
    unk4 = stream.read<typeof(unk4)>();
    unk5 = stream.read<typeof(unk5)>();

    if (stream_size > 0)
        external_data.parse(archives, stream);

    std::vector<std::uint8_t> embedded_data;
    embedded_data.resize(total_size);
    stream.read(embedded_data);

    const auto [fmt, fmt_bps] = format_mapper.at(pixel_format);

    unsigned int data_offset = 0;

    for (int current_mip = 0; current_mip < total_mips; current_mip++) {
        const auto current_mip_width = width >> current_mip;
        const auto current_mip_height = height >> current_mip;

        std::vector<std::uint8_t> image_data;
        image_data.resize(current_mip_width * current_mip_height * 4);

        if (current_mip == stream_mips)
            data_offset = 0;

        const auto data_ptr = (current_mip < stream_mips ? external_data.data().data() : embedded_data.data()) + data_offset;

#ifndef NDEBUG
        std::printf("Reading %s mip image %d-%d (pos: %u)\n", current_mip < stream_mips ? "external" : "embedded", current_mip_width, current_mip_height, data_offset);
#endif
        data_offset += current_mip_width * current_mip_height / (16 / fmt_bps);

        if (!decompress_texture(data_ptr, image_data, current_mip_width, current_mip_height, fmt)) {
            log("Texture::parse", "Texture was failed to decode");
            return;
        }

        unsigned int mip_id;
        glGenTextures(1, &mip_id);
        glBindTexture(GL_TEXTURE_2D, mip_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, current_mip_width, current_mip_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());

        image_mips.emplace_back(mip_id, std::move(image_data));
    }
}

Decima::Texture::~Texture() {
    for (const auto& [id, _] : image_mips) {
        glDeleteTextures(1, &id);
    }
}

static bool decompress_texture(const std::uint8_t* src, std::vector<std::uint8_t>& dst, int width, int height, int fmt) {
    detexTexture texture;
    texture.format = fmt;
    texture.data = const_cast<uint8_t*>(src);
    texture.width = width;
    texture.height = height;
    texture.width_in_blocks = int(width / (detexGetCompressedBlockSize(fmt) / 2));
    texture.height_in_blocks = int(height / (detexGetCompressedBlockSize(fmt) / 2));

    if (fmt == DETEX_TEXTURE_FORMAT_RGTC2 || fmt == DETEX_TEXTURE_FORMAT_BPTC || fmt == DETEX_TEXTURE_FORMAT_BC3) {
        texture.width_in_blocks *= 2;
        texture.height_in_blocks *= 2;
    }

    if (!detexDecompressTextureLinear(&texture, dst.data(), DETEX_PIXEL_FORMAT_RGBA8)) {
        std::printf("Buffer cannot be decompressed: %s\n", detexGetErrorMessage());
        return false;
    }

    return true;
}

#include <ostream>

namespace Decima {
    std::ostream& operator<<(std::ostream& os, Decima::TexturePixelFormat fmt) {
        switch (fmt) {
        case Decima::TexturePixelFormat::RGBA8:
            return os << "RGBA8";
        case Decima::TexturePixelFormat::A8:
            return os << "A8";
        case Decima::TexturePixelFormat::BC1:
            return os << "BC1";
        case Decima::TexturePixelFormat::BC2:
            return os << "BC2";
        case Decima::TexturePixelFormat::BC3:
            return os << "BC3";
        case Decima::TexturePixelFormat::BC4:
            return os << "BC4";
        case Decima::TexturePixelFormat::BC5:
            return os << "BC5";
        case Decima::TexturePixelFormat::BC7:
            return os << "BC7";
        default:
            return os << "Unsupported: " << std::to_string(int(fmt));
        }
    }
}

//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/core/texture.h"

#include <glad/glad.h>
#include <detex.h>

static bool decompress_texture(const std::vector<std::uint8_t>& src, std::vector<std::uint8_t>& dst, int width, int height, int fmt);

void Decima::Texture::parse(ArchiveArray& archives, Source& stream) {
    CoreFile::parse(archives, stream);
    uint64_t start = stream.tell();

    unk1 = stream.read<typeof(unk1)>();
    width = stream.read<typeof(width)>();
    height = stream.read<typeof(height)>();
    layers = stream.read<typeof(layers)>();
    mip_count = stream.read<typeof(mip_count)>();
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
        stream_info.parse(archives, stream);

    embedded_data.resize(total_size);
    stream.read_exact(embedded_data);

    /*
     * This is very clunky approach. Maybe rewrite into something
     * similar to what I did in ProjectDS::parse_core_file using
     * factory pattern
     */
    static const std::unordered_map<TexturePixelFormat, int> format_mapper = {
        { TexturePixelFormat::BC1, DETEX_TEXTURE_FORMAT_BC1 },
        { TexturePixelFormat::BC2, DETEX_TEXTURE_FORMAT_BC2 },
        { TexturePixelFormat::BC3, DETEX_TEXTURE_FORMAT_BC3 },
        { TexturePixelFormat::BC4, DETEX_TEXTURE_FORMAT_RGTC1 },
        { TexturePixelFormat::BC5, DETEX_TEXTURE_FORMAT_RGTC2 },
        { TexturePixelFormat::BC7, DETEX_TEXTURE_FORMAT_BPTC }
    };

    image_buffer.resize(width * height * 4);

    if (pixel_format == TexturePixelFormat::RGBA8) {
        if(stream_size > 0) {
            memcpy(image_buffer.data(), stream_info.data().data(), image_buffer.size());
        } else {
            memcpy(image_buffer.data(), embedded_data.data(), image_buffer.size());
        }
    } else {
        const auto format = format_mapper.find(pixel_format);

        if (format == format_mapper.end()) {
            std::stringstream buffer;
            buffer << "Image pixel format is not supported: " << pixel_format;
            return;
        }

        if (!decompress_texture(stream_info.data(), image_buffer, width, height, format->second)) {
            log("Texture::parse", "Texture was failed to decode");
            return;
        }
    }

    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer.data());
}

static bool decompress_texture(const std::vector<std::uint8_t>& src, std::vector<std::uint8_t>& dst, int width, int height, int fmt) {
    detexTexture texture;
    texture.format = fmt;
    texture.data = const_cast<uint8_t*>(src.data());
    texture.width = width;
    texture.height = height;
    texture.width_in_blocks = int(width / (detexGetCompressedBlockSize(fmt) / 2));
    texture.height_in_blocks = int(height / (detexGetCompressedBlockSize(fmt) / 2));

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

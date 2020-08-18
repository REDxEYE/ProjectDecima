//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/core/texture.h"

#include <glad/glad.h>

struct TexturePixelFormatInfo {
    /* This format's block size (in pixels) */
    int block_size;
    /* How many bits occupies one pixel */
    int block_density;
    /* Corresponding OpenGL internal format */
    int format;
    /* Is format whether compressed or not */
    bool compressed;
};

static const std::unordered_map<Decima::TexturePixelFormat, TexturePixelFormatInfo> format_info {
    // clang-format off
    { Decima::TexturePixelFormat::BC1,   { 4, 4,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, true  } },
    { Decima::TexturePixelFormat::BC3,   { 4, 8,  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, true  } },
    { Decima::TexturePixelFormat::BC4,   { 4, 4,  GL_COMPRESSED_RED_RGTC1,          true  } },
    { Decima::TexturePixelFormat::BC5,   { 4, 8,  GL_COMPRESSED_RG_RGTC2,           true  } },
    { Decima::TexturePixelFormat::BC7,   { 4, 8,  GL_COMPRESSED_RGBA_BPTC_UNORM,    true  } },
    { Decima::TexturePixelFormat::A8,    { 1, 8,  GL_R8,                            false } },
    { Decima::TexturePixelFormat::RGBA8, { 1, 32, GL_RGBA8,                         false } },
    // clang-format on
};

void Decima::Texture::parse(ArchiveArray& archives, Source& stream) {
    CoreFile::parse(archives, stream);
    unk1 = stream.read<decltype(unk1)>();
    width = stream.read<decltype(width)>();
    height = stream.read<decltype(height)>();
    layers = stream.read<decltype(layers)>();
    total_mips = stream.read<decltype(total_mips)>();
    pixel_format = stream.read<decltype(pixel_format)>();
    unk2 = stream.read<decltype(unk2)>();
    unk3 = stream.read<decltype(unk3)>();
    file_guid.parse(stream);
    buffer_size = stream.read<decltype(buffer_size)>();
    total_size = stream.read<decltype(total_size)>();
    stream_size = stream.read<decltype(stream_size)>();
    stream_mips = stream.read<decltype(stream_mips)>();
    unk4 = stream.read<decltype(unk4)>();
    unk5 = stream.read<decltype(unk5)>();

    if (stream_size > 0)
        external_data.parse(archives, stream);

    embedded_data.resize(total_size);
    stream.read_exact(embedded_data);

    if (const auto format = format_info.find(pixel_format); format != format_info.end()) {
        const auto [format_block_size, format_block_density, format_internal, format_compressed] = format->second;

        const std::uint8_t* stream_data = nullptr;

        for (auto mip = 0; mip < total_mips; mip++) {
            /*
             * Mips that are smaller than minimum block size
             * actually occupy size of one block and must be
             * cropped to the size of the mip, but we don't
             * do that just for the sake of simplicity.
             */
            const auto mip_width = std::max(width >> mip, format_block_size);
            const auto mip_height = std::max(height >> mip, format_block_size);
            const auto mip_buffer_size = mip_width * mip_height * format_block_density / 8;

            if (mip == 0)
                stream_data = external_data.data().data();

            if (mip >= stream_mips)
                stream_data = embedded_data.data();

            GLuint texture_id;
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (format_compressed) {
                glCompressedTexImage2D(GL_TEXTURE_2D, 0, format_internal, mip_width, mip_height, 0, mip_buffer_size, stream_data);
            } else {
                /*
                 * This is really only a specific case for RGBA8,
                 * but as far as we know there's more other unknown
                 * formats for us at this moment, so let this be a
                 * feature for the future (badum-tss).
                 */
                glTexImage2D(GL_TEXTURE_2D, 0, format_internal, mip_width, mip_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stream_data);
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            mip_textures.push_back(texture_id);
            stream_data += mip_buffer_size;
        }
    }
}

Decima::Texture::~Texture() {
    for (const auto texture_id : mip_textures) {
        glDeleteTextures(1, &texture_id);
    }
}

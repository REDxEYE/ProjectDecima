#include "decima/serializable/object/texture.hpp"

const std::unordered_map<Decima::TexturePixelFormat, Decima::TexturePixelFormatInfo> Decima::texture_format_info  {
    // clang-format off
    { Decima::TexturePixelFormat::BC1,     { 4, 4,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,      0,          true  } },
    { Decima::TexturePixelFormat::BC3,     { 4, 8,  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,      0,          true  } },
    { Decima::TexturePixelFormat::BC4,     { 4, 4,  GL_COMPRESSED_RED_RGTC1,               0,          true  } },
    { Decima::TexturePixelFormat::BC5,     { 4, 8,  GL_COMPRESSED_RG_RGTC2,                0,          true  } },
    { Decima::TexturePixelFormat::BC6,     { 4, 8,  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT, 0,          true  } },
    { Decima::TexturePixelFormat::BC7,     { 4, 8,  GL_COMPRESSED_RGBA_BPTC_UNORM,         0,          true  } },
    { Decima::TexturePixelFormat::A8,      { 1, 8,  GL_R8,                                 GL_RED,     false } },
    { Decima::TexturePixelFormat::RGBA8,   { 1, 32, GL_RGBA8,                              GL_RGBA,    false } },
    { Decima::TexturePixelFormat::RGBA16F, { 1, 64, GL_RGBA16F,                            GL_RGBA,    false } },
    // clang-format on
};

unsigned int Decima::TexturePixelFormatInfo::calculate_size(int width, int height) const  {
    return width * height * block_density;
}

unsigned int Decima::TexturePixelFormatInfo::create_texture(int width, int height, const void* data, std::size_t size) const {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (compressed) {
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, size, data);
    } else {
        /*
         * This is really only a specific case for RGBA8,
         * but as far as we know there's more other unknown
         * formats for us at this moment, so let this be a
         * feature for the future (badum-tss).
         */
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
}

void Decima::Texture::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    type = buffer.get<decltype(type)>();
    width = buffer.get<decltype(width)>();
    height = buffer.get<decltype(height)>();
    layers = buffer.get<decltype(layers)>();
    total_mips = buffer.get<decltype(total_mips)>();
    pixel_format = buffer.get<decltype(pixel_format)>();
    unk_0 = buffer.get<decltype(unk_0)>();
    unk_1 = buffer.get<decltype(unk_1)>();
    unk_2.parse(buffer, file);
    buffer_size = buffer.get<decltype(buffer_size)>();
    total_size = buffer.get<decltype(total_size)>();
    stream_size = buffer.get<decltype(stream_size)>();
    stream_mips = buffer.get<decltype(stream_mips)>();
    unk_3 = buffer.get<decltype(unk_3)>();
    unk_4 = buffer.get<decltype(unk_4)>();

    if (stream_size > 0)
        external_data.parse(manager, buffer, file);

    embedded_data.resize(total_size);

    /* TODO:
     *  By some reason, some textures contain
     *  total_size greater than actual embedded
     *  buffer. Add this workaround until we'll
     *  figure out what is happening.
     */

    buffer.get(embedded_data.data(), std::min(embedded_data.size(), buffer.size()));

    if (const auto format = texture_format_info.find(pixel_format); format != texture_format_info.end()) {
        const auto [format_block_size, format_block_density, format_type_internal, format_type_data, format_compressed] = format->second;

        const char* stream_data = nullptr;

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

            mip_textures.push_back(format->second.create_texture(mip_width, mip_height, stream_data, mip_buffer_size));
            stream_data += mip_buffer_size;
        }
    }
}

Decima::Texture::~Texture() {
    for (const auto texture_id : mip_textures) {
        glDeleteTextures(1, &texture_id);
    }
}

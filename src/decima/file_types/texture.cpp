//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/texture.h"

void Decima::Texture::parse(Source& stream) {
    CoreFile::parse(stream);
    uint64_t start = stream.tell();

    unk1 = stream.read<typeof(unk1)>();
    width = stream.read<typeof(width)>();
    height = stream.read<typeof(height)>();
    layers = stream.read<typeof(layers)>();
    mip_count = stream.read<typeof(mip_count)>();
    pixel_format = stream.read<typeof(pixel_format)>();
    unk2 = stream.read<typeof(unk2)>();
    unk3 = stream.read<typeof(unk3)>();
    file_guid = stream.read<typeof(file_guid)>();
    buffer_size = stream.read<typeof(buffer_size)>();
    total_size = stream.read<typeof(total_size)>();
    stream_size = stream.read<typeof(stream_size)>();
    unks[0] = stream.read<typeof(unks[0])>();
    unks[1] = stream.read<typeof(unks[1])>();
    unks[2] = stream.read<typeof(unks[2])>();

    if (stream_size != 0 && unks[0] != 0) {
        auto str_len = stream.read<uint32_t>();
        std::string buff(str_len, 0);
        stream.read(buff);
        stream_name = std::move(buff);
    }

    stream.seek(ash::seek_dir::beg, start + header.file_size - sizeof(GUID));
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

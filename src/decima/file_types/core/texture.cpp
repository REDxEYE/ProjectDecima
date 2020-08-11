//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/core/texture.h"

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

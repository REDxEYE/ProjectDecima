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
    file_guid[0] = stream.read<typeof(file_guid[0])>();
    file_guid[1] = stream.read<typeof(file_guid[1])>();
    buffer_size = stream.read<typeof(buffer_size)>();
    total_size = stream.read<typeof(total_size)>();
    unks[0] = stream.read<typeof(unks[0])>();
    unks[1] = stream.read<typeof(unks[0])>();
    unks[2] = stream.read<typeof(unks[0])>();
    unks[3] = stream.read<typeof(unks[0])>();

    if(unks[0]!=0&&unks[1]!=0){
        auto str_len = stream.read<uint32_t>();
        std::string buff(str_len,0);
        stream.read(buff);
        stream_name = std::move(buff);
    }

    stream.seek(ash::seek_dir::beg, start + header.file_size - sizeof(GUID));
}

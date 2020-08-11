//
// Created by MED45 on 11.08.2020.
//

#include "decima/file_types/core/texture_set.h"

void Decima::TextureSet::parse(Decima::ArchiveArray& archives, Decima::Source& stream) {
    CoreFile::parse(archives, stream);
    file_count = stream.read<uint32_t>();
    file_entries.resize(file_count);
    for (auto& file_entry : file_entries) {
        file_entry.parse(stream);
    }
    unk1 = stream.read<uint32_t>();
    src_count = stream.read<uint32_t>();
    src_entries.resize(src_count);
    for (auto& src_entry : src_entries) {
        src_entry.parse(stream);
    }
    unk2 = stream.read<uint8_t>();
}

void Decima::TextureGUIDEntry::parse(Decima::Source& stream) {
    unk1 = stream.read<uint32_t>();
    unk2 = stream.read<uint32_t>();
    unk3 = stream.read<uint32_t>();
    unk4 = stream.read<uint32_t>();
    unk5 = stream.read<uint16_t>();
    guid.parse(stream);
}

void Decima::SrcEntry::parse(Decima::Source& stream) {
    slot_id = stream.read<uint32_t>();
    src_name.parse(stream);
    unk1 = stream.read<uint16_t>();
    unk2[0] = stream.read<uint32_t>();
    unk2[1] = stream.read<uint32_t>();
    unk2[2] = stream.read<uint32_t>();
    if (unk1 == 0) {
        unk_pad = stream.read<uint32_t>();
    } else {
        width = stream.read<uint32_t>();
        height = stream.read<uint32_t>();
    }
    unk3[0] = stream.read<float>();
    unk3[1] = stream.read<float>();
    unk3[2] = stream.read<float>();
    unk3[3] = stream.read<float>();
}

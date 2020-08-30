//
// Created by MED45 on 11.08.2020.
//

#include "decima/file_types/core/texture_set.h"

void Decima::TextureSet::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);
    file_count = buffer.get<decltype(file_count)>();
    file_entries.resize(file_count);
    for (auto& file_entry : file_entries) {
        file_entry.parse(buffer);
    }
    unk1 = buffer.get<decltype(unk1)>();
    src_count = buffer.get<decltype(src_count)>();
    src_entries.resize(src_count);
    for (auto& src_entry : src_entries) {
        src_entry.parse(buffer);
    }
    unk2 = buffer.get<decltype(unk2)>();
}

void Decima::TextureGUIDEntry::parse(ash::buffer& buffer) {
    unk1 = buffer.get<decltype(unk1)>();
    unk2 = buffer.get<decltype(unk2)>();
    unk3 = buffer.get<decltype(unk3)>();
    unk4 = buffer.get<decltype(unk4)>();
    unk5 = buffer.get<decltype(unk5)>();
    guid.parse(buffer);
}

void Decima::SrcEntry::parse(ash::buffer& buffer) {
    slot_id = buffer.get<decltype(slot_id)>();
    src_name.parse(buffer);
    unk1 = buffer.get<decltype(unk1)>();
    buffer.get(unk2, sizeof(unk2));
    if (unk1 == 0) {
        unk_pad = buffer.get<decltype(unk_pad)>();
    } else {
        width = buffer.get<decltype(width)>();
        height = buffer.get<decltype(height)>();
    }
    buffer.get(unk3, sizeof(unk3));
}

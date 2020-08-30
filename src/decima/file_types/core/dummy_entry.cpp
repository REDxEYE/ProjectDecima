//
// Created by MED45 on 06.08.2020.
//

#include <decima/core_file.hpp>
#include "decima/file_types/core/entry_dummy.hpp"

void Decima::Dummy::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);
    buffer = buffer.skip(header.file_size - sizeof(Decima::GUID));
}

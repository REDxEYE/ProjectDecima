//
// Created by MED45 on 06.08.2020.
//

#include <decima/core_file.h>
#include "decima/file_types/core/dummy_entry.h"

void Decima::Dummy::parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) {
    CoreEntry::parse(archives, stream, nullptr);
    stream.seek(ash::seek_dir::cur, header.file_size - sizeof(Decima::GUID));
}

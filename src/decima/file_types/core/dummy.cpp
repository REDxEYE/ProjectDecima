//
// Created by MED45 on 06.08.2020.
//

#include "decima/file_types/core/dummy.h"

void Decima::Dummy::parse(ArchiveArray& archives, Source& stream) {
    CoreEntry::parse(archives, stream);
    stream.seek(ash::seek_dir::cur, header.file_size - sizeof(Decima::GUID));
}

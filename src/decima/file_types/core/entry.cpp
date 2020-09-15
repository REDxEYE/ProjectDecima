//
// Created by MED45 on 30.07.2020.
//

#include <iomanip>

#include "decima/file_types/core/entry.hpp"

namespace Decima {
    void CoreEntry::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
        header = buffer.get<decltype(header)>();
        guid.parse(buffer);
    }
}

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

    CoreHeader CoreEntry::peek_header(ash::buffer buffer) {
        return buffer.get<CoreHeader>();
    }

    std::string read_string(ash::buffer& buffer, const std::string& default_value) {
        const auto length = buffer.get<std::uint16_t>();

        if (length > 0) {
            std::string string(length, '\0');
            buffer.get(string);
            return string;
        }

        return default_value;
    }
}

//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/core/translation.hpp"

static std::string read_string(ash::buffer& buffer, const std::string& default_value = "<empty>") {
    const auto length = buffer.get<std::uint16_t>();

    if (length > 0) {
        std::string string(length, '\0');
        buffer.get(string);
        return string;
    }

    return default_value;
}

void Decima::Translation::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        translations[i] = read_string(buffer);
        comments[i] = read_string(buffer);
        flags[i] = buffer.get<char>();
    }
}

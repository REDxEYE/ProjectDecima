//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/core/translation.hpp"

void Decima::Translation::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        translations[i] = Decima::read_string(buffer);
        comments[i] = Decima::read_string(buffer);
        flags[i] = buffer.get<char>();
    }
}

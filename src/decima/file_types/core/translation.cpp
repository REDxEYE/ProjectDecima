//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/core/translation.hpp"

void Decima::Translation::parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) {
    CoreEntry::parse(archives, stream, nullptr);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        translations[i] = Decima::read_string(stream);
        comments[i] = Decima::read_string(stream);
        flags[i] = stream.read<std::uint8_t>();
    }
}

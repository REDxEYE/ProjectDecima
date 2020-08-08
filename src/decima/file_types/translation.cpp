//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/translation.hpp"

void Decima::Translation::parse(Source& stream) {
    CoreFile::parse(stream);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        translations[i] = Decima::read_string(stream);
        comments[i] = Decima::read_string(stream);
        flags[i] = stream.read<std::uint8_t>();
    }
}

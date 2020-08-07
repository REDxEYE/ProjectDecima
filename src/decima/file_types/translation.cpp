//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/translation.hpp"

void Decima::Translation::parse(std::istream& stream) {
    CoreFile::parse(stream);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        const auto translation = Decima::read_string(stream, "<empty>");
        const auto comment = Decima::read_string(stream, "<empty>");
        stream.seekg(1, std::ios::cur);

        translations[i] = std::move(translation);
        comments[i] = std::move(comment);
    }
}

void Decima::Translation::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    //TODO: ShadelesFox replace it with more appropriate code
    parse(imembuffer);
}

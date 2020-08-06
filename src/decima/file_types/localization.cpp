//
// Created by i.getsman on 06.08.2020.
//

#include "decima/file_types/localization.hpp"

void Decima::Localization::parse(std::istream& stream) {
    CoreFile::parse(stream);
    for (uint32_t i = 0; i < 25; i++) {
        uint16_t str_len;
        stream.read(reinterpret_cast<char*>(&str_len), 2);

        if (str_len > 0) {
            translations[i].resize(str_len);
            stream.read(translations[i].data(), str_len);
        } else {
            translations[i] = "<empty>";
        }

        stream.seekg(3, std::ios::cur);
    }
}

void Decima::Localization::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    //TODO: ShadelesFox replace it with more appropriate code
    parse(imembuffer);
}

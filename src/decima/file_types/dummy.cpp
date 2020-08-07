//
// Created by MED45 on 06.08.2020.
//

#include "decima/file_types/dummy.h"

void Decima::Dummy::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    parse(imembuffer);
}

void Decima::Dummy::parse(std::istream& stream) {
    CoreFile::parse(stream);
    stream.seekg(header.file_size - sizeof(Decima::GUID), std::ios::cur); //Skipping bytes
}

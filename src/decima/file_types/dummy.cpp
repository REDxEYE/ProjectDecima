//
// Created by MED45 on 06.08.2020.
//

#include "decima/file_types/dummy.h"
void Dummy::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    parse(imembuffer);
}
void Dummy::parse(std::istream& stream) {
    CoreFile::parse(stream);
    stream.seekg(header.file_size - 16 /*GUID*/, std::ios::cur); //Skipping bytes
}

//
// Created by MED45 on 07.08.2020.
//
#include "decima/file_types/texture.h"

void Decima::Texture::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    parse(imembuffer);
}
void Decima::Texture::parse(std::istream& stream) {
    CoreFile::parse(stream);
    uint64_t start = stream.tellg();
    stream.read(reinterpret_cast<char*>(&unk1), 4 + 4 + 4 + 16 + 4 + 4 + 16);

    stream.seekg(start + header.file_size - 16 /*GUID*/, std::ios::beg);
}

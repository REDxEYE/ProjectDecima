//
// Created by MED45 on 30.07.2020.
//

#include "decima/file_types/core.h"

namespace Decima {


    void CoreFile::parse(std::vector<uint8_t>& buffer) {
        imemstream imembuffer(buffer);
        parse(imembuffer);
//        parse();
    }

    void CoreFile::parse(std::istream& stream) {
        stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        stream.read(reinterpret_cast<char*>(&guid), 16);

    }

    uint64_t CoreFile::peek_header(std::vector<uint8_t>& buffer) {
        imemstream imembuffer(buffer);
        return peek_header(imembuffer);
    }

    uint64_t CoreFile::peek_header(std::istream& stream) {
        uint64_t magic;
        stream.read(reinterpret_cast<char*>(&magic), 8);
        stream.seekg(-8,std::ios::cur);
        return magic;
    }
}
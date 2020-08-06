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

    void CoreFile::parse(imemstream& stream) {
        stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        stream.read(reinterpret_cast<char*>(&guid), 16);

    }
}
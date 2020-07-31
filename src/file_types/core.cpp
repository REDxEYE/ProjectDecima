//
// Created by MED45 on 30.07.2020.
//

#include "file_types/core.h"

namespace Decima {

    void CoreFile::parse(std::istream& buffer) {
        buffer.read(reinterpret_cast<char*>(&header), sizeof(header));
        buffer.read(reinterpret_cast<char*>(&guid), 16);
    }

    void CoreFile::parse(std::vector<uint8_t>& buffer) {
        MemoryStream mem_buff(buffer);
        auto reader = mem_buff.as_stream();
        parse(reader);
    }
}
//
// Created by MED45 on 30.07.2020.
//

#include "file_types/core.h"

namespace Decima {


    void CoreFile::parse(MemoryStream& buffer) {
        auto reader = buffer.as_stream();
        reader.read(reinterpret_cast<char*>(&header), sizeof(header));
    }

    void CoreFile::parse(std::vector<uint8_t>& buffer) {
        MemoryStream mem_buff(buffer);
        parse(mem_buff);
    }
}
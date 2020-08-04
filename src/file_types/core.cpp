//
// Created by MED45 on 30.07.2020.
//

#include "file_types/core.h"

namespace Decima {


    void CoreFile::parse(std::vector<uint8_t>& buffer) {
        memcpy(&header,buffer.data(),sizeof(header));
        memcpy(&guid,buffer.data()+sizeof(header),16);
    }
}
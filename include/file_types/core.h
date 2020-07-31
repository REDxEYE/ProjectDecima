//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include <fstream>
#include "file_types/shared.hpp"
#include "memory_stream.h"

namespace Decima {



    class CoreFile {
        CoreHeader header = {};
        uint64_t guid[2]={0,0};
    public:
        virtual void parse(std::istream& buffer);

        virtual void parse(std::vector<uint8_t>& buffer);

    };
}

#endif //PROJECTDS_CORE_H

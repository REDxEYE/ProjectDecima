//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include "file_types/shared.hpp"
#include "memory_stream.h"

namespace Decima {



    class CoreFile {
        CoreHeader header = {};
    public:
        virtual void parse(MemoryStream& buffer);

        virtual void parse(std::vector<uint8_t>& buffer);

    };
}

#endif //PROJECTDS_CORE_H

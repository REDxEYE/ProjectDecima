//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include <fstream>
#include <vector>

#include "file_types/shared.hpp"


namespace Decima {

    struct __attribute__((packed)) CoreHeader {
        std::uint64_t filetype;
        std::uint32_t file_size;
    };

    class CoreFile {
    protected:
        CoreHeader header = {};
        uint64_t guid[2] = { 0, 0 };

    public:

        virtual void parse(std::vector<uint8_t>& buffer);
    };
}

#endif //PROJECTDS_CORE_H

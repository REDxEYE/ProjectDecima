//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include <fstream>
#include <vector>

#include "membuf.hpp"
#include "shared.hpp"

//#include "decima/archive/archive_array.h"

namespace Decima {
    struct GUID {
        std::uint64_t data[2];
    };

    std::ostream& operator<<(std::ostream& os, GUID guid);

    class ArchiveArray;
    struct __attribute__((packed)) CoreHeader {
        std::uint64_t filetype;
        std::uint32_t file_size;
    };

    class CoreFile {
    public:
        CoreHeader header {};
        GUID guid {};

        static uint64_t peek_header(std::vector<uint8_t>& buffer);

        static uint64_t peek_header(std::istream& stream);

        virtual void parse(std::vector<uint8_t>& buffer);

        virtual void parse(std::istream& stream);

        virtual void draw(ArchiveArray& archive_array);
    };
}

#endif //PROJECTDS_CORE_H

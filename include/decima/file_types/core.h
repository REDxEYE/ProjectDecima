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


namespace Decima {

    struct __attribute__((packed)) CoreHeader {
        std::uint64_t filetype;
        std::uint32_t file_size;
    };

    class CoreFile {
    public:
        CoreHeader header = {};

        uint64_t guid[2] = {0, 0};

        uint64_t peek_header(std::vector<uint8_t>& buffer);
        uint64_t peek_header(std::istream& stream);

        virtual void parse(std::vector<uint8_t>& buffer);

        virtual void parse(std::istream& stream);
    };
}

#endif //PROJECTDS_CORE_H

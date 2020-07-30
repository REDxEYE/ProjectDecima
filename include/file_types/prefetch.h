//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <string>
#include <vector>

#include "file_types/core.h"
#include "memory_stream.h"

namespace Decima {
    class Prefetch {
        struct __attribute__((packed)) Header {
            uint64_t unknown;
            uint32_t size;
            uint8_t filetype[16];
        };

        struct HashedString {
            uint32_t size;
            uint32_t hash;
            std::string string;
        };

        Header header = {};
        uint32_t string_count = 0;
        uint32_t file_sizes_count = 0;
        uint32_t indices_count = 0;

    public:

        std::vector<HashedString> strings;
        std::vector<uint32_t> file_sizes;
        std::vector<uint32_t> indices;

        Prefetch() = default;

        void parse(MemoryStream& buffer);

        void parse(std::vector<uint8_t>& buffer);
    };
}
#endif //PROJECTDS_CORE_H

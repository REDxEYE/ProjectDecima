//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_PREFETCH_H
#define PROJECTDS_PREFETCH_H

#include <string>
#include <vector>

#include "core.h"


namespace Decima {
    class Prefetch : public CoreFile {
    public:

        uint32_t string_count = 0;
        uint32_t file_sizes_count = 0;
        uint32_t indices_count = 0;


        std::vector<HashedString> strings;
        std::vector<uint32_t> file_sizes;
        std::vector<uint32_t> indices;

        Prefetch() = default;

        void parse(std::vector<uint8_t>& buffer) override;

        void parse(std::istream& stream) override;

    };
}
#endif //PROJECTDS_PREFETCH_H

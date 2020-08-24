//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_PREFETCH_H
#define PROJECTDS_PREFETCH_H

#include <string>
#include <vector>

#include "entry.h"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Prefetch : public CoreEntry {
    public:
        uint32_t string_count = 0;
        uint32_t file_sizes_count = 0;
        uint32_t indices_count = 0;

        std::vector<StringHashed> strings;
        std::vector<uint32_t> file_sizes;
        std::vector<uint32_t> indices;

        Prefetch() = default;

        void parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) override;
    };
}
#endif //PROJECTDS_PREFETCH_H

#pragma once

#include <string>
#include <vector>

#include "entry.hpp"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Prefetch : public CoreEntry {
    public:
        std::vector<StringHashed> strings;
        std::vector<uint32_t> file_sizes;
        std::vector<uint32_t> indices;

        Prefetch() = default;

        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
    };
}

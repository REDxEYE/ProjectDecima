#pragma once

#include <string>
#include <vector>

#include "entry.hpp"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Prefetch : public CoreEntry {
    public:
        std::vector<StringHashed> paths;
        std::vector<uint32_t> sizes;
        std::vector<uint32_t> indices;

        Prefetch() = default;

        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;
    };
}

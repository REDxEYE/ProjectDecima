#pragma once

#include <string>
#include <vector>

#include "object.hpp"
#include "decima/serializable/array.hpp"
#include "decima/serializable/string.hpp"

namespace Decima {
    class Prefetch : public CoreObject {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) override;
        void draw() override;

    public:
        Array<StringHashed> paths;
        Array<std::uint32_t> sizes;
        std::uint32_t links_total;
        std::vector<Array<std::uint32_t>> links;
    };
}

#pragma once

#include <string>
#include <vector>

#include "object.hpp"
#include "decima/serializable/array.hpp"
#include "decima/serializable/string.hpp"

namespace Decima {
    class Prefetch : public CoreObject {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;

    public:
        Array<StringHashed> paths;
        Array<uint32_t> sizes;
        Array<uint32_t> indices;
    };
}

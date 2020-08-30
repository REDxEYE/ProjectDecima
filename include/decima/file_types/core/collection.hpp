#pragma once

#include "decima/file_types/core/entry.hpp"
#include "decima/file_types/pod/reference.hpp"

namespace Decima {
    class Collection : public CoreEntry {
    public:
        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;

        std::vector<Reference> refs;
    };
}
#pragma once

#include "decima/file_types/core/core.h"
#include "decima/file_types/pod/reference.hpp"

namespace Decima {
    class Collection : public CoreEntry {
    public:
        void parse(ArchiveArray& archives, Source& stream) override;
        void draw() override;

        std::vector<Reference> refs;
    };
}
#pragma once

#include "entry.hpp"

namespace Decima {
    class Dummy : public Decima::CoreEntry {
    public:
        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
    };
}

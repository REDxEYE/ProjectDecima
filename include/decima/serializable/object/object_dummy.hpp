#pragma once

#include "decima/serializable/object/object.hpp"

namespace Decima {
    class Dummy : public Decima::CoreObject {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) override;
    };
}

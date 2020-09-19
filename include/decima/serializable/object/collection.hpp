#pragma once

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/array.hpp"
#include "decima/serializable/reference.hpp"

namespace Decima {
    class Collection : public CoreObject {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;

    public:
        Array<Ref> refs;
    };
}
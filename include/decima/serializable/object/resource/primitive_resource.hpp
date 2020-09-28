#pragma once

#include "decima/serializable/object/resource/resource.hpp"
#include "decima/serializable/object/common/bbox.hpp"
#include "decima/serializable/reference.hpp"

namespace Decima {
    class PrimitiveResource : public Resource {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) override;
        void draw() override;

    public:
        std::uint32_t flags;
        Ref vertex_array;
        Ref index_array;
        BBox3 bounding_box;
        Ref skd_tree;
        std::uint32_t start_index;
        std::uint32_t end_index;
        std::uint32_t hash;
    };
}

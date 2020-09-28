#include "decima/serializable/object/resource/primitive_resource.hpp"

void Decima::PrimitiveResource::parse(Decima::ArchiveManager& manager, ash::buffer& buffer, Decima::CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    flags = buffer.get<decltype(flags)>();
    vertex_array.parse(buffer, file);
    index_array.parse(buffer, file);
    bounding_box = buffer.get<decltype(bounding_box)>();
    skd_tree.parse(buffer, file);
    start_index = buffer.get<decltype(start_index)>();
    end_index = buffer.get<decltype(end_index)>();
    hash = buffer.get<decltype(hash)>();
}


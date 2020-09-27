#include "decima/serializable/object/resource/index_array_resource.hpp"

void Decima::IndexArrayResource::parse(Decima::ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    indices_count = buffer.get<decltype(indices_count)>();
    if (indices_count > 0) {
        flags = buffer.get<decltype(flags)>();
        type = buffer.get<decltype(type)>();
        is_streaming = buffer.get<decltype(is_streaming)>();
        resource_uuid.parse(buffer, file);
    }
}

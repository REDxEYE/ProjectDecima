#include "decima/serializable/object/resource/vertex_array_resource.hpp"

#include <algorithm>

void Decima::VertexStreamData::parse(ash::buffer& buffer, CoreFile& file) {
    offset = buffer.get<decltype(offset)>();
    storage_type = buffer.get<decltype(storage_type)>();
    slots_used = buffer.get<decltype(slots_used)>();
    element_type = buffer.get<decltype(element_type)>();
}

void Decima::VertexStreamInfo::parse(ash::buffer& buffer, CoreFile& file) {
    flags = buffer.get<decltype(flags)>();
    stride = buffer.get<decltype(stride)>();
    descriptors.parse(buffer, file);
    resource_uuid.parse(buffer, file);
}

void Decima::VertexArrayResource::parse(Decima::ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    vertex_count = buffer.get<decltype(vertex_count)>();
    vertex_stream_count = buffer.get<decltype(vertex_stream_count)>();
    is_streaming = buffer.get<decltype(is_streaming)>();
    vertex_stream_info.resize(vertex_stream_count);
    std::generate(vertex_stream_info.begin(), vertex_stream_info.end(), [&] {
        VertexStreamInfo info;
        info.parse(buffer, file);
        return info;
    });
}

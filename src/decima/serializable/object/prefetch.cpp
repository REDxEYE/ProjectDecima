#include "decima/serializable/object/prefetch.hpp"

void Decima::Prefetch::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) {
    CoreObject::parse(manager, buffer, nullptr);
    paths.parse(buffer);
    sizes.parse(buffer);
    indices.parse(buffer);
}

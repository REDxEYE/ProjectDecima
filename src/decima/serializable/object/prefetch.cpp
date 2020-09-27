#include "decima/serializable/object/prefetch.hpp"

void Decima::Prefetch::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    paths.parse(buffer, file);
    sizes.parse(buffer, file);
    indices.parse(buffer, file);
}

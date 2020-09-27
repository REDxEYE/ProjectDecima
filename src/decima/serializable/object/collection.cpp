#include "decima/serializable/object/collection.hpp"

void Decima::Collection::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    refs.parse(buffer, file);
}

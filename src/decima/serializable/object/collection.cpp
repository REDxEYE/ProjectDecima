#include "decima/serializable/object/collection.hpp"

void Decima::Collection::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) {
    CoreObject::parse(manager, buffer, nullptr);
    refs.parse(buffer);
}

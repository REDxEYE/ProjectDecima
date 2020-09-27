#include "decima/serializable/object/object_dummy.hpp"

void Decima::Dummy::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    buffer = buffer.skip(header.file_size - sizeof(Decima::GUID));
}

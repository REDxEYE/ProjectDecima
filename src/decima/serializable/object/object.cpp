#include "decima/serializable/object/object.hpp"

namespace Decima {
    void CoreObject::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) {
        header = buffer.get<decltype(header)>();
        guid.parse(buffer);
    }
}

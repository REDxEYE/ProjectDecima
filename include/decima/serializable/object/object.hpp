#pragma once

#include <cstdint>
#include <vector>

#include "decima/serializable/guid.hpp"
#include "decima/serializable/serializable.hpp"
#include "decima/shared.hpp"

class ProjectDS;

namespace Decima {
    class ArchiveManager;
    class CoreFile;

    DECIMA_PACK(struct CoreHeader {
        std::uint64_t file_type;
        std::uint32_t file_size;
    });

    class CoreObject : public CoreSerializable {
    public:
        virtual void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file);
        virtual void draw();

        inline static CoreHeader peek_header(ash::buffer buffer) {
            return buffer.get<CoreHeader>();
        }

    public:
        CoreHeader header;
        GUID guid;
    };
}

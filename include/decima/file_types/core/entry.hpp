#pragma once

#include <cstdint>
#include <vector>

#include "decima/shared.hpp"
#include "decima/file_types/pod/guid.hpp"

class ProjectDS;

namespace Decima {
    class ArchiveArray;
    class CoreFile;

    DECIMA_PACK(struct CoreHeader {
        std::uint64_t file_type;
        std::uint32_t file_size;
    });

    class CoreEntry {
    public:
        CoreHeader header {};
        GUID guid {};
        uint32_t offset {};

        static CoreHeader peek_header(ash::buffer buffer);

        virtual void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file);

        virtual void draw();
    };

    std::string read_string(ash::buffer& buffer, const std::string& default_value = "<empty>");
}

//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_ENTRY_H
#define PROJECTDS_ENTRY_H

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

        static uint64_t peek_header(Source& stream);

        virtual void parse(ArchiveArray& archives, Source& stream, CoreFile* core_file);

        virtual void draw();


    };

    std::string read_string(Source& stream, const std::string& default_value = "<empty>");
}
#endif //PROJECTDS_ENTRY_H
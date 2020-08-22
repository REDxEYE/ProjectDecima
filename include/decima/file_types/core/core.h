//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include <vector>

#include "decima/shared.hpp"
#include "decima/file_types/pod/guid.hpp"

class ProjectDS;

namespace Decima {
    class ArchiveArray;
    struct __attribute__((packed)) CoreHeader {
        std::uint64_t file_type;
        std::uint32_t file_size;
    };

    class CoreFile {
    public:
        CoreHeader header {};
        GUID guid {};
        uint32_t offset {};

        static uint64_t peek_header(Source& stream);

        virtual void parse(ArchiveArray& archives, Source& stream);

        virtual void draw();
    };

    std::string read_string(Source& stream, const std::string& default_value = "<empty>");
}
#endif //PROJECTDS_CORE_H

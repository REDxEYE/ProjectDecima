//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
#include <vector>

#include "ash.hpp"
#include "shared.hpp"

namespace Decima {
    struct GUID {
        std::uint64_t data[2];
    };

    std::ostream& operator<<(std::ostream& os, GUID guid);

    class ArchiveArray;
    struct __attribute__((packed)) CoreHeader {
        std::uint64_t file_type;
        std::uint32_t file_size;
    };

    class CoreFile {
    public:
        using Source = ash::buffered_source<std::vector<uint8_t>>;

        CoreHeader header {};
        GUID guid {};

        static uint64_t peek_header(Source& stream);

        virtual void parse(Source& stream);

        virtual void draw(ArchiveArray& archive_array);
    };

    std::string read_string(CoreFile::Source& stream, const std::string& default_value = "<empty>");
}
#endif //PROJECTDS_CORE_H

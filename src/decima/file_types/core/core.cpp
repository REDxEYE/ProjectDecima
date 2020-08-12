//
// Created by MED45 on 30.07.2020.
//

#include <iomanip>

#include "decima/file_types/core/core.h"

namespace Decima {
    void CoreFile::parse(ArchiveArray& archives, Source& stream) {
        header = stream.read<typeof(header)>();
        guid.parse(stream);
    }

    uint64_t CoreFile::peek_header(Source& stream) {
        auto magic = stream.read<std::uint64_t>();
        stream.seek(ash::seek_dir::cur, -8);
        return magic;
    }

    std::string read_string(Source& stream, const std::string& default_value) {
        const auto length = stream.read<std::uint16_t>();

        if (length > 0) {
            std::string buffer;
            buffer.resize(length);
            stream.read(buffer);
            return buffer;
        }

        return default_value;
    }
}
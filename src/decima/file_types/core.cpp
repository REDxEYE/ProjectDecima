//
// Created by MED45 on 30.07.2020.
//

#include <iomanip>

#include "decima/file_types/core.h"

namespace Decima {
    std::ostream& operator<<(std::ostream& os, GUID guid) {
        return os << std::hex << std::setfill('0')
                  << std::setw(8) << (guid.data[0] >> 32) << '-'
                  << std::setw(4) << (guid.data[0] >> 16 & 0xffff) << '-'
                  << std::setw(4) << (guid.data[0] >> 0 & 0xffff) << '-'
                  << std::setw(4) << (guid.data[1] >> 48) << '-'
                  << std::setw(12) << (guid.data[1] >> 0 & 0xffffffffffff);
    }

    void CoreFile::parse(Source& stream) {
        header = stream.read<typeof(header)>();
        guid = stream.read<typeof(guid)>();
    }

    uint64_t CoreFile::peek_header(Source& stream) {
        auto magic = stream.read<std::uint64_t>();
        stream.seek(ash::seek_dir::cur, -8);
        return magic;
    }

    std::string read_string(CoreFile::Source& stream, const std::string& default_value) {
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

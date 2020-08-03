//
// Created by MED45 on 03.08.2020.
//

#ifndef PROJECTDS_ARCHIVE_FILE_H
#define PROJECTDS_ARCHIVE_FILE_H

#include <cstdint>

namespace Decima {
    class File {
    public:
        std::uint8_t* mio_start;
        std::uint8_t* mio_end;
        std::uint64_t hash;
        std::uint32_t size;

        std::vector<std::uint8_t> unpack(uint32_t size); //TODO: size min = find minimal size
    private:
        std::vector<std::uint8_t> decrypt(uint32_t size); //TODO: size = any
    };
}

#endif //PROJECTDS_ARCHIVE_FILE_H

//
// Created by MED45 on 30.07.2020.
//

#ifndef PROJECTDS_CORE_H
#define PROJECTDS_CORE_H

#include <cstdint>
namespace Decima {
    struct CoreHeader {
        std::uint64_t filetype;
        std::uint32_t file_size;
    };

    class CoreFile {
    };
}

#endif //PROJECTDS_CORE_H

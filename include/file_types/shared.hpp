//
// Created by i.getsman on 30.07.2020.
//

#ifndef PROJECTDS_SHARED_HPP
#define PROJECTDS_SHARED_HPP

#include "unordered_map"
#include "string"

namespace Decima {

    static std::unordered_map<uint64_t, std::string> known_file_types = {
            {0xA664164D69FD2B38, "Texture"}
    };

    struct __attribute__((packed)) CoreHeader {
        std::uint64_t filetype;
        std::uint32_t file_size;
    };

    struct HashedString {
        uint32_t size;
        uint32_t hash;
        std::string string;
    };

}

#endif //PROJECTDS_SHARED_HPP

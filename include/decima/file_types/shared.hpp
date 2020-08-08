//
// Created by i.getsman on 30.07.2020.
//

#ifndef PROJECTDS_SHARED_HPP
#define PROJECTDS_SHARED_HPP

#include <unordered_map>
#include <string>

namespace Decima {

    struct HashedString {
        uint32_t size;
        uint32_t hash;
        std::string string;
    };

}

#endif //PROJECTDS_SHARED_HPP

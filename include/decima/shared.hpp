//
// Created by i.getsman on 30.07.2020.
//

#ifndef PROJECTDS_SHARED_HPP
#define PROJECTDS_SHARED_HPP

#include "ash.hpp"

namespace Decima {
    using Source = ash::buffered_source<std::vector<uint8_t>>;
}

#ifdef _MSC_VER
#define DECIMA_PACK(_Def) __pragma(pack(push, 1)) _Def __pragma(pack(pop))
#else
#define DECIMA_PACK(_Def) _Def __attribute__((packed))
#endif

#endif //PROJECTDS_SHARED_HPP

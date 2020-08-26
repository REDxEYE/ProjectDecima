//
// Created by i.getsman on 30.07.2020.
//

#ifndef PROJECTDS_SHARED_HPP
#define PROJECTDS_SHARED_HPP

#include <optional>
#include <type_traits>

#include "ash.hpp"

namespace Decima {
    using Source = ash::buffered_source<std::vector<uint8_t>>;

    template <class T>
    using OptionalRef = std::optional<std::reference_wrapper<T>>;
}

#ifdef _MSC_VER
    #define DECIMA_PACK(_Def) __pragma(pack(push, 1)) _Def __pragma(pack(pop))
#else
    #define DECIMA_PACK(_Def) _Def __attribute__((packed))
#endif

#define __DECIMA_STR2(s) #s
#define __DECIMA_STR1(s) __DECIMA_STR2(s)

#define DECIMA_VERSION_MAJOR 0
#define DECIMA_VERSION_MINOR 1
#define DECIMA_VERSION_MICRO 5

// clang-format off
#define DECIMA_VERSION \
    __DECIMA_STR1(DECIMA_VERSION_MAJOR) "." \
    __DECIMA_STR1(DECIMA_VERSION_MINOR) "." \
    __DECIMA_STR1(DECIMA_VERSION_MICRO)
// clang-format on

#endif //PROJECTDS_SHARED_HPP

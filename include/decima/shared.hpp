#pragma once

#include <iostream>
#include <optional>
#include <type_traits>

namespace Decima {
    constexpr static uint8_t cipher_seed = 0x2A;
    constexpr static uint32_t plain_cipher_key[4] = { 0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E };
    constexpr static uint32_t chunk_cipher_key[4] = { 0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F };

    template <class T>
    using OptionalRef = std::optional<std::reference_wrapper<T>>;

    template <typename T>
    std::string to_string(const T&);
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
#define DECIMA_VERSION_MICRO 8

// clang-format off
#define DECIMA_VERSION \
    __DECIMA_STR1(DECIMA_VERSION_MAJOR) "." \
    __DECIMA_STR1(DECIMA_VERSION_MINOR) "." \
    __DECIMA_STR1(DECIMA_VERSION_MICRO)
// clang-format on

#define DECIMA_LOG(...) (_decima_log(_decima_filename(__FILE__), ':', __LINE__, ' ', __VA_ARGS__))

template <typename... Args>
inline constexpr void _decima_log(Args&&... args) {
    (std::cout << ... << args) << '\n';
}

template <std::size_t Size>
inline constexpr const char* _decima_filename(const char (&path)[Size]) {
    std::size_t separator_offset = 0;

    for (std::size_t index = 0; index < Size; index++) {
        if (path[index] == '/' || path[index] == '\\') {
            separator_offset = index + 1;
        }
    }

    return path + separator_offset;
}
#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <Kraken.h>
#include <MurmurHash3.h>

std::string uint64_to_hex(uint64_t value);

uint64_t hash_string(const std::string& filename, uint8_t seed);

std::string sanitize_name(const std::string& filename);

bool decompress_chunk_data(const uint8_t* data, uint64_t data_size, uint64_t decompressed_size, uint8_t* output);

void split(const std::string& str, std::vector<std::string>& cont, char delim);

#include <array>
#include <cmath>
#include <iomanip>

inline std::string format_size(std::size_t size) noexcept {
    using FormatInfo = std::pair<std::string, std::size_t>;

    static const std::vector<FormatInfo> formats {
        FormatInfo { "B", 0 },
        FormatInfo { "KB", 2 },
        FormatInfo { "MB", 1 },
        FormatInfo { "GB", 1 },
        FormatInfo { "TB", 1 }
    };

    const auto format_order = std::min<std::size_t>(formats.size(), std::log(size) / 6.931471805599453);
    const auto [format_name, format_precision] = formats[format_order];

    std::stringstream buffer;
    buffer << std::fixed << std::setprecision(format_precision)
           << (size / std::pow(1024, format_order))
           << ' ' << format_name;

    return buffer.str();
}

#include <iostream>

template <typename... Args>
inline constexpr void log(Args&&... args) {
    (std::cout << ... << args) << '\n';
}

template <std::size_t Size>
inline constexpr const char* filename(const char (&path)[Size]) {
    std::size_t separator_offset = 0;

    for (std::size_t index = 0; index < Size; index++) {
        if (path[index] == '/' || path[index] == '\\') {
            separator_offset = index + 1;
        }
    }

    return path + separator_offset;
}

#define LOG(...) (log(filename(__FILE__), ':', __LINE__, ' ', __VA_ARGS__))

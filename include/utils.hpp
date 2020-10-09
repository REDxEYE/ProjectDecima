#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <Kraken.h>
#include <MurmurHash3.h>

std::string uint64_to_hex(uint64_t value);

uint64_t hash_string(const std::string& filename, uint8_t seed);

std::string sanitize_name(std::string filename);

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

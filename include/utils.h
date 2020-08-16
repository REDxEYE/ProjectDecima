//
// Created by MED45 on 25.07.2020.
//

#ifndef PROJECTDS_UTILS_H
#define PROJECTDS_UTILS_H

#include <string>
#include <vector>
#include <sstream>

#include <Kraken.h>
#include <MurmurHash3.h>

std::string uint64_to_hex(uint64_t value);

uint64_t hash_string(const std::string& filename, uint8_t seed);

std::string sanitize_name(const std::string& filename);

uint64_t calculate_first_containing_chunk(uint64_t file_offset, int32_t chunk_size);

uint64_t calculate_last_containing_chunk(uint64_t file_offset, int32_t file_size, int32_t chunk_size);

bool decompress_chunk_data(const std::vector<uint8_t>& data, uint64_t decompressed_size, uint8_t* output);

bool decompress_chunk_data(const uint8_t* data, uint64_t data_size, uint64_t decompressed_size, uint8_t* output);

void split(const std::string& str, std::vector<std::string>& cont, char delim);

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

#endif //PROJECTDS_UTILS_H

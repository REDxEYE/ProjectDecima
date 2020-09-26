//
// Created by MED45 on 26.07.2020.
//
#include <cstdint>
#include <filesystem>

#include "utils.hpp"
#include <iostream>

std::string uint64_to_hex(uint64_t value) {
    char hash[32];
    sprintf(hash, "%llx", value);
    return std::string(hash);
}

uint64_t hash_string(const std::string& filename, uint8_t seed) {
    std::uint64_t hash[2];
    MurmurHash3_x64_128(filename.c_str(), (int32_t)filename.size() + 1, seed, &hash);
    return hash[0];
}

std::string sanitize_name(const std::string& filename) {
    const auto extension = filename.substr(filename.rfind('.') + 1);

    if (extension != "stream" && extension != "core")
        return filename + ".core";

    return filename;
}

void split(const std::string& str, std::vector<std::string>& cont, char delim) {
    std::size_t offset = 0;

    for (std::size_t index = 0; index < str.size(); index++) {
        if (str[index] == delim) {
            cont.emplace_back(str.substr(offset, index - offset));
            offset = index + 1;
        }
    }

    cont.emplace_back(str.substr(offset));
}

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

void split(const std::string& str, std::vector<std::string>& cont, char delim);

#endif //PROJECTDS_UTILS_H


//
// Created by MED45 on 26.07.2020.
//

#include <file_types/prefetch.h>

void Decima::Prefetch::parse(std::vector<uint8_t>& buffer) {
    //TODO: ShadelesFox replace it with more appropriate code
    CoreFile::parse(buffer);
    uint64_t ptr = 16 + sizeof(header);
    memcpy(&string_count, &buffer[ptr], sizeof(string_count));
    ptr += sizeof(string_count);
    strings.resize(string_count);

    for (uint32_t i = 0; i < string_count; i++) {
        auto& string = strings[i];

        memcpy(&string.size, &buffer[ptr], 8);
        ptr += 8;

        string.string.resize(string.size);

        memcpy(string.string.data(), &buffer[ptr], strings[i].size);
        ptr += strings[i].size;
    }

    memcpy(&file_sizes_count, &buffer[ptr], sizeof(file_sizes_count));
    ptr += sizeof(file_sizes_count);

    file_sizes.resize(file_sizes_count);
    memcpy(file_sizes.data(), &buffer[ptr], 4 * file_sizes_count);
    ptr += 4 * file_sizes_count;
    memcpy(&indices_count, &buffer[ptr], sizeof(indices_count));
    ptr += sizeof(indices_count);
    indices.resize(indices_count);
    uint64_t left = buffer.size()-ptr;
    for (uint32_t i = 0; i < indices_count; i++) {
        indices[i] = ((uint32_t*)&buffer[ptr])[i];
    }
}

//
// Created by MED45 on 26.07.2020.
//

#include <file_types/prefetch.h>
#include "archive/decima_archive.h"
#include <utils.h>


void Decima::Prefetch::parse(MemoryStream &buffer) {
    auto reader = buffer.as_stream();
    reader.read(reinterpret_cast<char*>(&header), sizeof(header));

    reader.read(reinterpret_cast<char*>(&string_count), sizeof(string_count));
    strings.resize(string_count);

    for (uint32_t i = 0; i < string_count; i++) {
        auto &string = strings[i];
        reader.read(reinterpret_cast<char*>(&string.size), 4);
        reader.read(reinterpret_cast<char*>(&string.hash), 4);
        string.string.resize(string.size);
        reader.read(string.string.data(), strings[i].size);

    }

    reader.read(reinterpret_cast<char*>(&file_sizes_count), sizeof(file_sizes_count));
    file_sizes.resize(file_sizes_count);

    reader.read(reinterpret_cast<char*>(file_sizes.data()), 4 * file_sizes_count);

    reader.read(reinterpret_cast<char*>(&indices_count), sizeof(indices_count));
    indices.resize(indices_count);

    reader.read(reinterpret_cast<char*>(indices.data()), 4 * indices_count);


}

void Decima::Prefetch::parse(std::vector<uint8_t> &buffer) {
    MemoryStream mem_buff(buffer);
    parse(mem_buff);


}

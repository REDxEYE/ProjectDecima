//
// Created by MED45 on 26.07.2020.
//

#include <decima/file_types/prefetch.h>

void Decima::Prefetch::parse(std::vector<uint8_t>& buffer) {
    imemstream imembuffer(buffer);
    //TODO: ShadelesFox replace it with more appropriate code
    parse(imembuffer);
}

void Decima::Prefetch::parse(imemstream& stream) {
    CoreFile::parse(stream);
    stream.read(reinterpret_cast<char*>(&string_count), sizeof(string_count));

    strings.resize(string_count);

    for (uint32_t i = 0; i < string_count; i++) {
        auto& hashed_string = strings[i];
        stream.read(reinterpret_cast<char*>(&hashed_string.size), 8);
        hashed_string.string.resize(hashed_string.size);
        stream.read(hashed_string.string.data(), hashed_string.size);
    }
    stream.read(reinterpret_cast<char*>(&file_sizes_count), sizeof(file_sizes_count));
    file_sizes.resize(file_sizes_count);
    stream.read(reinterpret_cast<char*>(file_sizes.data()), 4 * file_sizes_count);

    stream.read(reinterpret_cast<char*>(&indices_count), sizeof(indices_count));
    indices.resize(indices_count);
    stream.read(reinterpret_cast<char*>(indices.data()), 4 * indices_count);
}

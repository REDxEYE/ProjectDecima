//
// Created by MED45 on 26.07.2020.
//

#include <decima/file_types/prefetch.h>


void Decima::Prefetch::parse(Source& stream) {
    CoreFile::parse(stream);
    string_count = stream.read<typeof(string_count)>();
    strings.resize(string_count);

    for (uint32_t i = 0; i < string_count; i++) {
        auto& hashed_string = strings[i];
        hashed_string.size = stream.read<typeof(hashed_string.size)>();
        hashed_string.hash = stream.read<typeof(hashed_string.hash)>();
        hashed_string.string.resize(hashed_string.size);
        stream.read_exact(hashed_string.string);
    }

    file_sizes_count = stream.read<typeof(file_sizes_count)>();
    file_sizes.resize(file_sizes_count);
    stream.read_exact(file_sizes);

    indices_count = stream.read<typeof(indices_count)>();
    indices.resize(file_sizes_count);
    stream.read_exact(indices);
}

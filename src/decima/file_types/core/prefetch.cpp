//
// Created by MED45 on 26.07.2020.
//

#include "decima/file_types/core/prefetch.h"

void Decima::Prefetch::parse(ArchiveArray& archives, Source& stream) {
    CoreEntry::parse(archives, stream);
    string_count = stream.read<decltype(string_count)>();
    strings.resize(string_count);

    std::for_each_n(strings.begin(), string_count, [&](auto& string) {
        string.parse(stream);
    });

    file_sizes_count = stream.read<decltype(file_sizes_count)>();
    file_sizes.resize(file_sizes_count);
    stream.read_exact(file_sizes);

    indices_count = stream.read<decltype(indices_count)>();
    indices.resize(file_sizes_count);
    stream.read_exact(indices);
}

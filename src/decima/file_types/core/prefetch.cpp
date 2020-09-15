//
// Created by MED45 on 26.07.2020.
//

#include <decima/core_file.hpp>
#include "decima/file_types/core/prefetch.hpp"

void Decima::Prefetch::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);

    paths.resize(buffer.get<std::uint32_t>());
    std::for_each(paths.begin(), paths.end(), [&](auto& string) { string.parse(buffer); });

    sizes.resize(buffer.get<std::uint32_t>());
    buffer.get(sizes);

    indices.resize(buffer.get<std::uint32_t>());
    buffer.get(indices);
}


#include "decima/file_types/core/collection.hpp"

void Decima::Collection::parse(ArchiveArray& archives, Source& stream) {
    CoreFile::parse(archives, stream);

    const auto count = stream.read<std::uint32_t>();

    for (auto index = 0; index < count; index++) {
        Reference ref;
        ref.parse(archives, stream);
        refs.push_back(ref);
    }
}

void Decima::Collection::draw() {
    for (auto& ref : refs) {
        ref.draw();
    }
}

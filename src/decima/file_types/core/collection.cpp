
#include "decima/file_types/core/collection.hpp"

void Decima::Collection::parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) {
    CoreEntry::parse(archives, stream, nullptr);

    const auto count = stream.read<std::uint32_t>();

    for (auto index = 0; index < count; index++) {
        Reference ref;
        ref.parse(stream);
        refs.push_back(ref);
        ref.add_ref(&ref,core_file);
    }
}

void Decima::Collection::draw() {
    for (auto& ref : refs) {
        ref.draw();
    }
}

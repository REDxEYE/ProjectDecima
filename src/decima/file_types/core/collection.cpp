
#include "decima/file_types/core/collection.hpp"

void Decima::Collection::parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) {
    CoreEntry::parse(archives, buffer, nullptr);

    const auto count = buffer.get<std::uint32_t>();

    for (auto index = 0; index < count; index++) {
        Reference ref;
        ref.parse(buffer);
        refs.push_back(ref);
        ref.add_ref(&ref, core_file);
    }
}

void Decima::Collection::draw() {
    for (auto& ref : refs) {
        ref.draw();
    }
}

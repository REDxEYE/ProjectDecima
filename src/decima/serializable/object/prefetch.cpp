#include "decima/serializable/object/prefetch.hpp"

#include <algorithm>

void Decima::Prefetch::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);
    paths.parse(buffer, file);
    sizes.parse(buffer, file);
    links_total = buffer.get<decltype(links_total)>();
    std::generate_n(std::back_inserter(links), paths.data().size(), [&] {
        Array<std::uint32_t> link;
        link.parse(buffer, file);
        return std::move(link);
    });
}

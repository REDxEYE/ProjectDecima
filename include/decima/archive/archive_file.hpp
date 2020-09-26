#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>

#include <util/mio.hpp>

#include "decima/serializable/object/object_dummy.hpp"
#include "decima/serializable/reference.hpp"

namespace Decima {
    class Archive;
    class ArchiveFileEntry;

    class CoreFile {
    public:
        CoreFile(Archive& archive, ArchiveFileEntry& entry, mio::mmap_source& source);

        void parse(ArchiveManager& archive_array);

    private:
        Archive& archive;
        ArchiveFileEntry& entry;

    public:
        std::vector<char> contents;
        std::vector<std::pair<std::shared_ptr<CoreObject>, std::size_t>> objects;
    };
}

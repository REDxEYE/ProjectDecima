#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>
#include <util/mio.hpp>

#include "decima/serializable/object/object_dummy.hpp"
#include "decima/serializable/reference.hpp"

#include "util/buffer.hpp"

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
        std::vector<std::shared_ptr<CoreObject>> objects;
    };
}

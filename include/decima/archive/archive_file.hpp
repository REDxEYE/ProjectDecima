#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>

#include <util/mio.hpp>

#include "decima/serializable/object/object_dummy.hpp"

namespace Decima {
    class Archive;
    class ArchiveFileEntry;
    class Ref;

    class CoreFile {
    public:
        CoreFile(Archive& archive, ArchiveManager& manager, ArchiveFileEntry& entry, mio::mmap_source& source);

        void parse();

        void queue_reference(Ref*);
        void resolve_reference(const std::shared_ptr<CoreObject>&);
        void resolve_reference(const CoreFile&);

    private:
        Archive& archive;
        ArchiveManager& manager;
        ArchiveFileEntry& entry;

    public:
        std::vector<char> contents;
        std::vector<std::pair<std::shared_ptr<CoreObject>, std::size_t>> objects;
        std::vector<Ref*> references;
    };
}

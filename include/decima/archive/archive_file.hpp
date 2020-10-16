#pragma once

#include <memory>
#include <vector>

namespace Decima {
    class Archive;
    class ArchiveManager;
    class ArchiveFileEntry;
    class CoreObject;
    class Ref;

    class CoreFile {
    public:
        CoreFile(Archive& archive, ArchiveManager& manager, ArchiveFileEntry& entry, std::ifstream& source);

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

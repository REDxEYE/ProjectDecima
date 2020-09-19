#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>

#include "decima/serializable/object/object_dummy.hpp"
#include "decima/serializable/reference.hpp"

#include "util/buffer.hpp"

namespace Decima {
    class Archive;
    class ArchiveFileEntry;
    class ArchiveChunkEntry;

    class CoreFile {
    public:
        CoreFile(ArchiveFileEntry* file_entry, ash::buffer file_buffer, Archive* archive, bool encrypted);

        ArchiveFileEntry* file_entry;
        ash::buffer file_buffer;
        Archive* archive;
        bool encrypted;

        std::vector<char> storage;

        std::vector<std::shared_ptr<CoreObject>> entries;

        void parse(ArchiveManager& archive_array);

        [[nodiscard]] inline bool is_valid() const { return file_entry != nullptr; };

        void unpack();

    private:
        std::pair<std::vector<ArchiveChunkEntry>::iterator, std::vector<ArchiveChunkEntry>::iterator>
        get_chunk_boundaries();

        [[nodiscard]] std::uint64_t chunk_id_by_offset(uint64_t offset) const;

        static void decrypt_chunk(uint8_t* data, const ArchiveChunkEntry& chunk_entry);
    };
}

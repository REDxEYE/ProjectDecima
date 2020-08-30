#pragma once

#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>

#include "decima/archive/archive_structs.hpp"
#include "decima/file_types/core/entry.hpp"
#include "decima/file_types/pod/reference.hpp"

#include "util/buffer.hpp"

namespace Decima {
    class Archive;

    class CoreFile {
    public:
        CoreFile(FileEntry* file_entry, ash::buffer file_buffer, Archive* archive, bool encrypted);

        FileEntry* file_entry;
        ash::buffer file_buffer;
        Archive* archive;
        bool encrypted;

        std::vector<char> storage;

        std::vector<std::shared_ptr<CoreEntry>> entries;

        void parse(ArchiveArray& archive_array);

        [[nodiscard]] inline bool is_valid() const { return file_entry != nullptr; };

        void unpack();

    private:
        std::pair<std::vector<Decima::ChunkEntry>::iterator, std::vector<Decima::ChunkEntry>::iterator>
        get_chunk_boundaries();

        [[nodiscard]] std::uint64_t chunk_id_by_offset(uint64_t offset) const;

        static void decrypt_chunk(uint8_t* data, const Decima::ChunkEntry& chunk_entry);
    };
}

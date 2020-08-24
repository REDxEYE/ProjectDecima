//
// Created by MED45 on 03.08.2020.
//

#ifndef PROJECTDS_CORE_FILE_H
#define PROJECTDS_CORE_FILE_H


#include <cstdint>
#include <cmath>
#include <vector>

#include "mio.hpp"

#include "decima/archive/archive_structs.hpp"
#include "decima/file_types/core/entry.h"
#include "decima/file_types/pod/reference.hpp"

namespace Decima {
    class Archive;

    class CoreFile {
    public:
        CoreFile(FileEntry* file_entry_, mio::mmap_source* filebuffer_, Archive* archive_, bool encrypted_);

        CoreFile() = default;

        FileEntry* file_entry = nullptr;
        mio::mmap_source* filebuffer = nullptr;
        Archive* archive = nullptr;
        bool encrypted = true;

        std::vector<uint8_t> storage;

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

#endif //PROJECTDS_CORE_FILE_H

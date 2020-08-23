//
// Created by MED45 on 03.08.2020.
//

#ifndef PROJECTDS_ARCHIVE_FILE_H
#define PROJECTDS_ARCHIVE_FILE_H

#include <cstdint>
#include <vector>

#include "archive_structs.hpp"
#include "mio.hpp"
#include "archive.h"

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

        [[nodiscard]] inline bool is_valid() const { return file_entry != nullptr; };
        void unpack();
        void decrypt();
        void get_raw();

    private:
        std::pair<std::vector<Decima::ChunkEntry>::iterator, std::vector<Decima::ChunkEntry>::iterator>
        get_chunk_boundaries() {
            auto& chunk_table = archive->chunk_table;
            if (file_id == -1)
                return { chunk_table.end(), chunk_table.end() };

            const auto file_offset = file_entry.offset;
            const auto file_size = file_entry.size;

            const auto first_chunk = calculate_first_containing_chunk(file_offset, header.max_chunk_size);
            const auto last_chunk = calculate_last_containing_chunk(file_offset, file_size, header.max_chunk_size);

            const auto first_chunk_row = chunk_id_by_offset(first_chunk);
            const auto last_chunk_row = chunk_id_by_offset(last_chunk);

            return { chunk_table.begin() + first_chunk_row, chunk_table.begin() + last_chunk_row + 1 };
        }
    };

}

#endif //PROJECTDS_ARCHIVE_FILE_H

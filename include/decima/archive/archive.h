//
// Created by MED45 on 25.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_H
#define PROJECTDS_ARCHIVE_H

#include <vector>
#include <fstream>

#include "mio.hpp"
#include "decima/constants.hpp"
#include "archive_structs.hpp"
#include "archive_file.h"

namespace Decima {


    class ArchiveArray;

    class Archive {
        mio::mmap_source filebuffer;
        std::vector<Decima::ChunkEntry> chunk_table;

    public:
        std::vector<Decima::FileEntry> content_table;
        std::string filepath;
        Decima::ArchiveHeader header = {};

        Archive(const std::string& workdir, const std::string& filename);

        Archive(const std::string& workdir, uint64_t filehash);

        bool open();

        [[nodiscard]] bool is_encrypted() const;

        [[nodiscard]] bool is_valid() const;

        [[maybe_unused]] Decima::CompressedFile query_file(uint64_t file_hash);

        [[maybe_unused]] Decima::CompressedFile query_file(const std::string& file_name);

    private:
        static void decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data);

        uint64_t chunk_id_by_offset(uint64_t offset);

        std::pair<std::vector<ChunkEntry>::iterator, std::vector<ChunkEntry>::iterator>
        get_mio_boundaries(int32_t file_id);

        [[maybe_unused]] [[nodiscard]] uint64_t get_file_index(uint64_t file_hash) const;

        [[maybe_unused]] [[nodiscard]] uint64_t get_file_index(const std::string& file_name) const;

        friend ArchiveArray;
    };


}
#endif //PROJECTDS_ARCHIVE_H

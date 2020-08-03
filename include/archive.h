//
// Created by MED45 on 25.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_H
#define PROJECTDS_ARCHIVE_H

#include <vector>
#include <fstream>

#include "mio.hpp"
#include "constant.hpp"

namespace Decima {

    namespace structs {
        struct ArchiveHeader {
            Version version; //0x20304050
            uint32_t key;
            uint64_t file_size;
            uint64_t data_size;
            uint64_t content_table_size;
            uint32_t chunk_table_size;
            uint32_t max_chunk_size;
        };

        struct FileEntry {
            uint32_t entry_num;
            uint32_t key;
            uint64_t hash;
            uint64_t offset;
            uint32_t size;
            uint32_t key2;
        };

        struct ChunkEntry {
            uint64_t uncompressed_offset; //relative offset once uncompressed
            uint32_t uncompressed_size;
            uint32_t key_1;
            uint64_t compressed_offset;
            uint32_t compressed_size;
            uint32_t key_2;
        };
    }

    class ArchiveArray;

    class Archive {
        mio::mmap_source filebuffer;
        std::vector<structs::ChunkEntry> chunk_table;

    public:
        std::vector<structs::FileEntry> content_table;
        std::string filepath;
        structs::ArchiveHeader header = {};

        Archive(const std::string& workdir, const std::string& filename);

        Archive(const std::string& workdir, uint64_t filehash);

        bool open();

        [[nodiscard]] bool is_encrypted() const;

        [[nodiscard]] bool is_valid() const;

        std::vector<uint8_t> query_file(uint32_t file_hash);
        std::vector<uint8_t> query_file(const std::string& file_name);

    private:
        static void decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data);

        uint64_t find_chunk_by_offset(uint64_t offset);

        std::vector<uint8_t> get_chunk_data(const structs::ChunkEntry& chunk);

        void decrypt_chunk(uint32_t chunk_id, std::vector<uint8_t>& src);

        std::vector<uint8_t> extract_file_data(int32_t file_id);

        [[nodiscard]] uint64_t get_file_index(uint64_t file_hash) const;

        [[nodiscard]] uint64_t get_file_index(const std::string& file_name) const;

        friend ArchiveArray;
    };

};
#endif //PROJECTDS_ARCHIVE_H
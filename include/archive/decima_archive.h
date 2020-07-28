//
// Created by MED45 on 25.07.2020.
//

#ifndef PROJECTDS_DECIMA_ARCHIVE_H
#define PROJECTDS_DECIMA_ARCHIVE_H

#include <vector>
#include <fstream>

namespace Decima {
    class ArchiveArray;

    static uint32_t magic = 0x20304050;
    static uint32_t encrypted_magic = 0x21304050;

    static uint8_t seed = 0x2A;
    static uint32_t murmur_salt[4] = {0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E};
    static uint32_t murmur_salt2[4] = {0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F};

    struct Header {
        uint32_t magic; //0x20304050
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

    class Archive {
        std::ifstream filebuffer;
        std::vector<ChunkEntry> chunk_table;
    public:
        std::vector<FileEntry> content_table;
        std::string filepath;
        Header header = {0};

        Archive(const std::string &workdir, const std::string &filename);

        Archive(const std::string &workdir, uint64_t filehash);

        bool open();

        void read_content_table();

        void read_chunk_table();

        bool is_encrypted() const;

        bool is_valid() const;

        void get_file_data(uint32_t file_id, std::vector<uint8_t> &data_out);

        void get_file_data(const std::string &file_id, std::vector<uint8_t> &data_out);

        uint64_t get_file_id(uint64_t file_hash) const;

        uint64_t get_file_id(const std::string &file_name) const;

    private:
        static void decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data);

        uint64_t find_chunk_by_offset(uint64_t offset);

        void get_chunk_data(ChunkEntry &chunk, std::vector<uint8_t> &data);

        void decrypt_chunk(uint32_t chunk_id, std::vector<uint8_t> &src);

        friend ArchiveArray;
    };

};
#endif //PROJECTDS_DECIMA_ARCHIVE_H

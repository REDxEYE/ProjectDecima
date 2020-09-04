#pragma once

#include "decima/constants.hpp"

namespace Decima {

    struct ArchiveHeader {
        Version version; //0x20304050
        uint32_t key;
    };

    struct ArchiveContentInfo {

        uint64_t file_size;
        uint64_t data_size;
        uint64_t content_table_size;
        uint32_t chunk_table_size;
        uint32_t max_chunk_size;
    };

    struct FileEntry {
        uint32_t entry_num;
        uint32_t key_0;
        uint64_t hash;
        uint64_t offset;
        uint32_t size;
        uint32_t key_1;
    };
    struct ChunkEntry {
        uint64_t uncompressed_offset; //relative offset once uncompressed
        uint32_t uncompressed_size;
        uint32_t key_0;
        uint64_t compressed_offset;
        uint32_t compressed_size;
        uint32_t key_1;
    };
}

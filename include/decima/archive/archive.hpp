#pragma once

#include <vector>
#include <util/mio.hpp>

#include "decima/constants.hpp"
#include "decima/core_file.hpp"

#include "util/buffer.hpp"

namespace Decima {
    struct ArchiveHeader {
        FileType type;
        uint32_t key;
    };

    struct ArchiveContentInfo {
        std::uint64_t file_size;
        std::uint64_t data_size;
        std::uint64_t content_table_size;
        std::uint32_t chunk_table_size;
        std::uint32_t max_chunk_size;
    };

    struct ArchiveFileEntry {
        std::uint32_t entry_num;
        std::uint32_t key_0;
        std::uint64_t hash;
        std::uint64_t offset;
        std::uint32_t size;
        std::uint32_t key_1;
    };

    struct ArchiveChunkEntry {
        std::uint64_t uncompressed_offset;
        std::uint32_t uncompressed_size;
        std::uint32_t key_0;
        std::uint64_t compressed_offset;
        std::uint32_t compressed_size;
        std::uint32_t key_1;
    };

    class Archive {
    public:
        explicit Archive(const std::string& path);

        [[nodiscard]] OptionalRef<Decima::CoreFile> query_file(std::uint64_t hash);

        std::string path;
        Decima::ArchiveHeader header {};
        Decima::ArchiveContentInfo content_info {};
        std::vector<Decima::ArchiveChunkEntry> chunk_table;
        std::vector<Decima::ArchiveFileEntry> content_table;

    private:
        friend class ArchiveManager;
        friend class CoreFile;

        bool open();

        std::unordered_map<std::uint64_t, Decima::CoreFile> m_cache;
        std::unordered_map<std::uint64_t, std::uint64_t> m_hash_to_index;
        mio::mmap_source m_stream;
    };
}

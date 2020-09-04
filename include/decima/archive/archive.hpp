#pragma once

#include <vector>
#include <util/mio.hpp>

#include "decima/archive/archive_structs.hpp"
#include "decima/constants.hpp"
#include "decima/core_file.hpp"

#include "util/buffer.hpp"

namespace Decima {
    class Archive {
    public:
        explicit Archive(const std::string& path);

        [[nodiscard]] OptionalRef<Decima::CoreFile> query_file(std::uint64_t hash);

        std::string path;
        Decima::ArchiveHeader header {};
        Decima::ArchiveContentInfo content_info {};
        std::vector<Decima::ChunkEntry> chunk_table;
        std::vector<Decima::FileEntry> content_table;

    private:
        friend class ArchiveArray;
        friend class CoreFile;

        bool open();

        std::unordered_map<std::uint64_t, Decima::CoreFile> m_cache;
        std::unordered_map<std::uint64_t, std::uint64_t> m_hash_to_index;
        mio::mmap_source m_stream;
    };
}

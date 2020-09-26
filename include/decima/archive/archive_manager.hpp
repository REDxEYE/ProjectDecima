#pragma once

#include <unordered_map>

#include "decima/archive/archive.hpp"
#include "decima/serializable/object/prefetch.hpp"

namespace Decima {
    class ArchiveManager {
    public:
        void load_archive(const std::string& path);
        void load_prefetch();

        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(const std::string& name);

        [[nodiscard]] Decima::OptionalRef<Decima::ArchiveFileEntry> get_file_entry(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::ArchiveFileEntry> get_file_entry(const std::string& name);

        std::unordered_map<uint64_t, uint32_t> hash_to_archive_index;
        std::unordered_map<uint64_t, std::string> hash_to_name;

        std::vector<Archive> manager;
    };
}

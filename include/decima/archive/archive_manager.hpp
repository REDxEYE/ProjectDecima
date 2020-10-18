#pragma once

#include <unordered_map>
#include <memory>

#include "decima/archive/archive.hpp"
#include "decima/serializable/object/prefetch.hpp"
#include "decima/shared.hpp"
#include "util/compressor.hpp"

namespace Decima {
    class CoreFile;

    class ArchiveManager {
    public:
        void load_archive(const std::string& path);
        void load_prefetch();

        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(const std::string& name);

        [[nodiscard]] Decima::OptionalRef<Decima::ArchiveFileEntry> get_file_entry(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::ArchiveFileEntry> get_file_entry(const std::string& name);

        std::unordered_map<uint64_t, uint32_t> hash_to_archive_index;

        // TODO: GUI-related, must be removed
        std::unordered_map<std::uint64_t, std::string> hash_to_name;
        std::unordered_map<std::uint64_t, std::uint64_t> hash_to_index;

        std::vector<Archive> archives;
        std::unique_ptr<Decima::Compressor> compressor;
        std::unique_ptr<Decima::Prefetch> prefetch;
    };
}

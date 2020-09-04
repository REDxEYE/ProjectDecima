#pragma once

#include "tracy_memdbg.hpp"


#include <optional>
#include <unordered_map>

#include "decima/archive/archive.hpp"
#include "decima/file_types/core/prefetch.hpp"

namespace Decima {
    class ArchiveArray {
    public:
        explicit ArchiveArray(std::string  directory);

        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::CoreFile> query_file(const std::string& name);

        [[nodiscard]] Decima::OptionalRef<Decima::FileEntry> get_file_entry(std::uint64_t hash);
        [[nodiscard]] Decima::OptionalRef<Decima::FileEntry> get_file_entry(const std::string& name);

        std::unordered_map<uint64_t, uint32_t> hash_to_archive_index;
        std::unordered_map<uint64_t, std::string> hash_to_name;

        std::vector<Archive> archives;

    private:
        std::string m_directory;

        void open();
        void read_prefetch_file();
    };

}

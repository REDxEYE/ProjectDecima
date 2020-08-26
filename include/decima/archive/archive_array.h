//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_ARRAY_H
#define PROJECTDS_ARCHIVE_ARRAY_H

#include <optional>
#include <unordered_map>

#include "decima/archive/archive.h"
#include "decima/file_types/core/prefetch.h"

namespace Decima {
    class ArchiveArray {
    public:
        explicit ArchiveArray(const std::string& directory);

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

#endif //PROJECTDS_ARCHIVE_ARRAY_H

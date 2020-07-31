//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_ARRAY_H
#define PROJECTDS_ARCHIVE_ARRAY_H

#include <unordered_map>
#include "file_types/prefetch.h"
#include "decima_archive.h"

namespace Decima {
    class ArchiveArray {

        std::string workdir;

        Prefetch prefetch;

    public:
        std::unordered_map<uint64_t, uint32_t> hash_to_archive;
        std::unordered_map<uint64_t, std::string> hash_to_name;

        std::vector<Archive> archives;

        explicit ArchiveArray(const std::string& _workdir);

        ArchiveArray() = default;

        void open(const std::string& _workdir);


        std::vector<uint8_t> query_file(uint64_t file_hash);
        std::vector<uint8_t> query_file(const std::string& file_name);

        std::optional<std::reference_wrapper<Decima::structs::FileEntry>> get_file_entry(const std::string& file_name);

        std::optional<std::reference_wrapper<Decima::structs::FileEntry>> get_file_entry(uint64_t file_hash);

        void read_prefetch_file();
    };

}

#endif //PROJECTDS_ARCHIVE_ARRAY_H

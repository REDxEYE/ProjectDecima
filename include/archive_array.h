//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_ARRAY_H
#define PROJECTDS_ARCHIVE_ARRAY_H

#include <unordered_map>
#include "file_types/prefetch.h"
#include "archive/decima_archive.h"

namespace Decima {
    class ArchiveArray {

        std::string workdir;

        Prefetch prefetch;

    public:
        std::unordered_map<uint32_t, uint32_t> hash_to_archive;
        std::unordered_map<uint32_t, std::string> hash_to_name;

        std::vector<Archive> archives;

        explicit ArchiveArray(const std::string &_workdir);

        ArchiveArray() = default;

        void open(const std::string &_workdir);

        void read_content_table();

        void get_file_data(uint64_t file_hash, std::vector<uint8_t> &data_out);

        void get_file_data(const std::string &file_id, std::vector<uint8_t> &data_out);

        FileEntry* get_file_entry(const std::string &file_name);

        FileEntry* get_file_entry(uint64_t file_hash);

        void read_prefetch_file();
    };

}

#endif //PROJECTDS_ARCHIVE_ARRAY_H

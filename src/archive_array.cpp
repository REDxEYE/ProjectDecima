//
// Created by MED45 on 26.07.2020.
//

#include "archive_array.h"

#include <utility>
#include <filesystem>
#include <iostream>
#include <optional>
#include <utils.h>

Decima::ArchiveArray::ArchiveArray(const std::string &_workdir) {
    open(_workdir);
}

void Decima::ArchiveArray::read_content_table() {
    uint32_t archive_id = 0;
    for (auto &archive:archives) {
        archive.read_content_table();
        archive.read_chunk_table();
        for (auto &entry:archive.content_table) {
            hash_to_archive[entry.hash] = archive_id;
        }
        archive_id++;
    }
}

void Decima::ArchiveArray::get_file_data(uint64_t file_hash, std::vector<uint8_t> &data_out) {
    auto archive_id = hash_to_archive.find(file_hash);
    if (archive_id != hash_to_archive.end()) {
        auto &archive = archives[archive_id->second];
        archive.get_file_data(archive.get_file_id(file_hash), data_out);
    }
}

void Decima::ArchiveArray::get_file_data(const std::string &file_id, std::vector<uint8_t> &data_out) {
    uint64_t hash = hash_string(sanitize_name(file_id), seed);
    get_file_data(hash, data_out);
}

void Decima::ArchiveArray::read_prefetch_file() {
    std::vector<uint8_t> prefetch_data;
    get_file_data("prefetch/fullgame.prefetch", prefetch_data);
    prefetch.parse(prefetch_data);

    for (auto &string:prefetch.strings) {
        auto hash = hash_string(sanitize_name(string.string), seed);
        hash_to_name[hash] = string.string;
    }

}

void Decima::ArchiveArray::open(const std::string &_workdir) {
    workdir = _workdir;
    for (auto &file:std::filesystem::directory_iterator(workdir,
                                                        std::filesystem::directory_options::skip_permission_denied)) {
        archives.emplace_back(workdir, file.path().filename().string());
    }

    for (auto &archive:archives) {
        archive.open();
    }
}

Decima::FileEntry* Decima::ArchiveArray::get_file_entry(const std::string &file_name) {
    auto hash = hash_string(sanitize_name(file_name), seed);
    return get_file_entry(hash);
}

Decima::FileEntry* Decima::ArchiveArray::get_file_entry(uint64_t file_hash) {
    if (hash_to_archive.find(file_hash) != hash_to_archive.end()) {
        uint64_t archive_id = hash_to_archive.at(file_hash);
        auto &archive = archives[archive_id];
        uint64_t file_id = archive.get_file_id(file_hash);
        return &archive.content_table[file_id];
    }
    return {};
}


//
// Created by MED45 on 26.07.2020.
//

#include "archive_array.h"

#include <utility>
#include <filesystem>
#include <iostream>
#include <utils.h>

Decima::ArchiveArray::ArchiveArray(const std::string& _workdir){
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
    auto tmp = std::filesystem::path(file_id);
    if (tmp.extension() != ".core") { tmp = tmp.replace_extension(tmp.extension().string() + ".core"); }
    uint64_t hash = hash_string(tmp.string(), seed);
    get_file_data(hash, data_out);
}

void Decima::ArchiveArray::read_prefetch_file() {
    std::vector<uint8_t> prefetch_data;
    get_file_data("prefetch/fullgame.prefetch", prefetch_data);
    prefetch.parse(prefetch_data);

    for (auto &string:prefetch.strings) {
        auto hash = hash_string(string.string + ".core", seed);
        hash_to_name[hash] = string.string;
    }

}

void Decima::ArchiveArray::open(const std::string& _workdir) {
    workdir = _workdir;
    for (auto &file:std::filesystem::directory_iterator(workdir,
                                                        std::filesystem::directory_options::skip_permission_denied)) {
        archives.emplace_back(workdir, file.path().filename().string());
    }

    for (auto &archive:archives) {
        archive.open();
    }
}


//
// Created by MED45 on 26.07.2020.
//

#include <filesystem>
#include <iostream>
#include <optional>

#include "utils.h"
#include "archive_array.h"

Decima::ArchiveArray::ArchiveArray(const std::string& _workdir) {
    open(_workdir);
}

void Decima::ArchiveArray::read_prefetch_file() {
    std::vector<uint8_t> prefetch_data = query_file("prefetch/fullgame.prefetch");

    prefetch.parse(prefetch_data);

    for (auto& string : prefetch.strings) {
        uint64_t hash = hash_string(sanitize_name(string.string), seed);
        hash_to_name[hash] = string.string;
    }
}

void Decima::ArchiveArray::open(const std::string& _workdir) {
    workdir = _workdir;
    for (auto& file : std::filesystem::directory_iterator(workdir,
             std::filesystem::directory_options::skip_permission_denied)) {
        archives.emplace_back(workdir, file.path().filename().string());
    }
    uint32_t archive_id = 0;
    for (auto& archive : archives) {
        archive.open();
        for (auto& entry : archive.content_table) {
            hash_to_archive[entry.hash] = archive_id;
        }
        archive_id++;
    }
    read_prefetch_file();
}

std::optional<std::reference_wrapper<Decima::structs::FileEntry>>
Decima::ArchiveArray::get_file_entry(const std::string& file_name) {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return get_file_entry(hash);
}

std::optional<std::reference_wrapper<Decima::structs::FileEntry>>
Decima::ArchiveArray::get_file_entry(uint64_t file_hash) {
    if (hash_to_archive.find(file_hash) != hash_to_archive.end()) {
        uint64_t archive_id = hash_to_archive.at(file_hash);
        auto& archive = archives[archive_id];
        uint64_t file_id = archive.get_file_index(file_hash);
        return std::optional<std::reference_wrapper<Decima::structs::FileEntry>> { archive.content_table[file_id] };
    }
    return std::nullopt;
}

std::vector<uint8_t> Decima::ArchiveArray::query_file(uint64_t file_hash) {
    auto archive_id = hash_to_archive.find(file_hash);
    if (archive_id != hash_to_archive.end()) {
        auto& archive = archives[archive_id->second];
        return std::move(archive.extract_file_data(archive.get_file_index(file_hash)));
    }
    return {};
}

std::vector<uint8_t> Decima::ArchiveArray::query_file(const std::string& file_name) {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return std::move(query_file(hash));
}
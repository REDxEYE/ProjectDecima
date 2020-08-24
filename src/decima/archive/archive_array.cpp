//
// Created by MED45 on 26.07.2020.
//

#include <filesystem>
#include <iostream>
#include <optional>
#include <thread>
#include <mutex>

#include "utils.h"
#include "decima/archive/archive_array.h"

Decima::ArchiveArray::ArchiveArray(const std::string& _workdir) {
    open(_workdir);
}

void Decima::ArchiveArray::read_prefetch_file() {

    auto prefetch_data = query_file("prefetch/fullgame.prefetch");
    prefetch_data.unpack();

    Source source(prefetch_data.storage, 1024);
    prefetch.parse(*this, source, nullptr);

    for (const auto& string : prefetch.strings) {
        uint64_t hash = hash_string(sanitize_name(string.data()), seed);
        hash_to_name.insert({ hash, string.data() });
    }

    hash_to_name.insert({ 0x2fff5af65cd64c0a, "prefetch/fullgame.prefetch" });
}

void Decima::ArchiveArray::open(const std::string& _workdir) {
    workdir = _workdir;
    for (auto& file : std::filesystem::directory_iterator(workdir,
             std::filesystem::directory_options::skip_permission_denied)) {
        archives.emplace_back(workdir, file.path().filename().string());
    }

    std::vector<std::thread> thread_pool;
    std::mutex mutex;

    for(std::size_t index = 0; index < archives.size(); index++) {
        auto& archive = archives[index];

        LOG("Loading archive ", std::filesystem::path(archive.filepath).stem().string(), " (", std::to_string(index + 1), '/', std::to_string(archives.size()), ')');

        thread_pool.emplace_back([&](const auto id) {
            archive.open();

            std::lock_guard<std::mutex> lock { mutex };

            for (const auto& entry : archive.content_table) {
                hash_to_archive.emplace(entry.hash, id);
            }
        }, index);
    }

    for (auto& thread : thread_pool) {
        thread.join();
    }

    read_prefetch_file();
}

std::optional<std::reference_wrapper<Decima::FileEntry>>
Decima::ArchiveArray::get_file_entry(const std::string& file_name) {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return get_file_entry(hash);
}

std::optional<std::reference_wrapper<Decima::FileEntry>>
Decima::ArchiveArray::get_file_entry(uint64_t file_hash) {
    if (hash_to_archive.find(file_hash) != hash_to_archive.end()) {
        uint64_t archive_id = hash_to_archive.at(file_hash);
        auto& archive = archives[archive_id];
        uint64_t file_id = archive.get_file_index(file_hash);
        return std::optional<std::reference_wrapper<FileEntry>> { archive.content_table[file_id] };
    }
    return std::nullopt;
}

Decima::CoreFile Decima::ArchiveArray::query_file(uint64_t file_hash) {
    auto archive_id = hash_to_archive.find(file_hash);
    if (archive_id != hash_to_archive.end()) {
        auto& archive = archives[archive_id->second];
        return std::move(archive.query_file(file_hash));
    }
    return Decima::CoreFile(nullptr, nullptr, nullptr, true);
}

Decima::CoreFile Decima::ArchiveArray::query_file(const std::string& file_name) {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return std::move(query_file(hash));
}
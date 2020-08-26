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

Decima::ArchiveArray::ArchiveArray(const std::string& directory)
    : m_directory(directory) { open(); }

void Decima::ArchiveArray::read_prefetch_file() {
    auto& prefetch_data = query_file("prefetch/fullgame.prefetch").value().get();
    prefetch_data.unpack();

    Source source(prefetch_data.storage, 1024);

    Prefetch prefetch;
    prefetch.parse(*this, source, nullptr);

    for (const auto& string : prefetch.strings) {
        uint64_t hash = hash_string(sanitize_name(string.data()), seed);
        hash_to_name.insert({ hash, string.data() });
    }

    hash_to_name.insert({ 0x2fff5af65cd64c0a, "prefetch/fullgame.prefetch" });
}

void Decima::ArchiveArray::open() {
    for (const auto& file : std::filesystem::directory_iterator(m_directory)) {
        archives.emplace_back(file.path().string());
    }

    std::vector<std::thread> thread_pool;
    std::mutex mutex;

    for (std::size_t index = 0; index < archives.size(); index++) {
        auto& archive = archives[index];

        LOG("Loading archive ", std::filesystem::path(archive.path).stem().string(), " (", std::to_string(index + 1), '/', std::to_string(archives.size()), ')');

        thread_pool.emplace_back([&](const auto id) {
            archive.open();

            std::lock_guard<std::mutex> lock { mutex };

            for (const auto& entry : archive.content_table) {
                hash_to_archive_index.emplace(entry.hash, id);
            }
        },
            index);
    }

    for (auto& thread : thread_pool) {
        thread.join();
    }

    read_prefetch_file();
}

Decima::OptionalRef<Decima::FileEntry> Decima::ArchiveArray::get_file_entry(std::uint64_t hash) {
    if (auto archive_id = hash_to_archive_index.find(hash); archive_id != hash_to_archive_index.end()) {
        auto& archive = archives.at(archive_id->second);
        auto file_id = archive.m_hash_to_index.find(hash)->second;

        return std::make_optional(std::ref(archive.content_table.at(file_id)));
    }

    return {};
}

Decima::OptionalRef<Decima::FileEntry> Decima::ArchiveArray::get_file_entry(const std::string& name) {
    return get_file_entry(hash_string(sanitize_name(name), seed));
}

Decima::OptionalRef<Decima::CoreFile> Decima::ArchiveArray::query_file(std::uint64_t hash) {
    if (auto archive_index = hash_to_archive_index.find(hash); archive_index != hash_to_archive_index.end())
        return archives.at(archive_index->second).query_file(hash);

    return {};
}

Decima::OptionalRef<Decima::CoreFile> Decima::ArchiveArray::query_file(const std::string& name) {
    return query_file(hash_string(sanitize_name(name), seed));
}
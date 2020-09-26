//
// Created by MED45 on 26.07.2020.
//

#include <filesystem>
#include <optional>

#include "utils.hpp"
#include "decima/archive/archive_manager.hpp"

void Decima::ArchiveManager::load_archive(const std::string& path) {
    auto& archive = archives.emplace_back(path);
    archive.open();

    for (const auto& entry : archive.file_entries) {
        hash_to_archive_index.emplace(entry.hash, static_cast<int>(archives.size() - 1));
    }
}

void Decima::ArchiveManager::load_prefetch() {
    auto& prefetch_data = query_file("prefetch/fullgame.prefetch").value().get();

    ash::buffer buffer(prefetch_data.contents.data(), prefetch_data.contents.size());

    Prefetch prefetch;
    prefetch.parse(*this, buffer, nullptr);

    for (const auto& string : prefetch.paths.data()) {
        hash_to_name.emplace(hash_string(sanitize_name(string.data()), cipher_seed), string.data());
    }

    hash_to_name.emplace(0x2fff5af65cd64c0a, "prefetch/fullgame.prefetch");
}

Decima::OptionalRef<Decima::ArchiveFileEntry> Decima::ArchiveManager::get_file_entry(std::uint64_t hash) {
    if (auto archive_id = hash_to_archive_index.find(hash); archive_id != hash_to_archive_index.end()) {
        auto& archive = archives.at(archive_id->second);
        auto file_id = archive.m_hash_to_index.find(hash)->second;

        return std::make_optional(std::ref(archive.file_entries.at(file_id)));
    }

    return {};
}

Decima::OptionalRef<Decima::ArchiveFileEntry> Decima::ArchiveManager::get_file_entry(const std::string& name) {
    return get_file_entry(hash_string(sanitize_name(name), cipher_seed));
}

Decima::OptionalRef<Decima::CoreFile> Decima::ArchiveManager::query_file(std::uint64_t hash) {
    if (auto archive_index = hash_to_archive_index.find(hash); archive_index != hash_to_archive_index.end())
        return archives.at(archive_index->second).query_file(hash);

    return {};
}

Decima::OptionalRef<Decima::CoreFile> Decima::ArchiveManager::query_file(const std::string& name) {
    return query_file(hash_string(sanitize_name(name), cipher_seed));
}

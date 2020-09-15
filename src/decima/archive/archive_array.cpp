//
// Created by MED45 on 26.07.2020.
//

#include <filesystem>
#include <optional>

#include "utils.hpp"
#include "decima/archive/archive_array.hpp"

void Decima::ArchiveArray::load_archive(const std::string& path) {
    LOG("Loading archive ", std::filesystem::path(path).stem().string());

    auto& archive = archives.emplace_back(path);
    archive.open();

    for (const auto& entry : archive.content_table) {
        hash_to_archive_index.emplace(entry.hash, archives.size() - 1);
    }
}

void Decima::ArchiveArray::load_prefetch() {
    auto& prefetch_data = query_file("prefetch/fullgame.prefetch").value().get();
    prefetch_data.unpack();

    ash::buffer buffer(prefetch_data.storage.data(), prefetch_data.storage.size());

    Prefetch prefetch;
    prefetch.parse(*this, buffer, nullptr);

    for (const auto& string : prefetch.strings) {
        hash_to_name.emplace(hash_string(sanitize_name(string.data()), seed), string.data());
    }

    hash_to_name.emplace(0x2fff5af65cd64c0a, "prefetch/fullgame.prefetch");
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

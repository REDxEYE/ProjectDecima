//
// Created by MED45 on 25.07.2020.
//

#include <MurmurHash3.h>

#include "decima/archive/archive.hpp"

static void decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data) {
    const std::uint32_t key[8] = {
        key_1, Decima::encryption_key_1[1], Decima::encryption_key_1[2], Decima::encryption_key_1[3],
        key_2, Decima::encryption_key_1[1], Decima::encryption_key_1[2], Decima::encryption_key_1[3]
    };

    std::uint32_t iv[8];
    MurmurHash3_x64_128(key, 16, Decima::seed, iv);
    MurmurHash3_x64_128(key + 4, 16, Decima::seed, iv + 4);

    data[0] ^= iv[0];
    data[1] ^= iv[1];
    data[2] ^= iv[2];
    data[3] ^= iv[3];
    data[4] ^= iv[4];
    data[5] ^= iv[5];
    data[6] ^= iv[6];
    data[7] ^= iv[7];
}

Decima::Archive::Archive(const std::string& path)
    : m_stream(path)
    , path(path) { open(); }

bool Decima::Archive::open() {
    memcpy(&header, m_stream.data(), sizeof(ArchiveHeader));

    if (header.type != ArchiveType::Regular && header.type != ArchiveType::Encrypted)
        return false;

    if (header.type == ArchiveType::Encrypted)
        decrypt(header.key, header.key + 1, (uint32_t*)&header.file_size);

    std::size_t read_offset = sizeof(ArchiveHeader);

    file_entries.resize(header.file_entries_count);
    memcpy(file_entries.data(), m_stream.data() + read_offset, sizeof(ArchiveFileEntry) * header.file_entries_count);

    read_offset += sizeof(ArchiveFileEntry) * header.file_entries_count;

    chunk_entries.resize(header.chunk_entries_count);
    memcpy(chunk_entries.data(), m_stream.data() + read_offset, sizeof(chunk_entries.front()) * header.chunk_entries_count);

    if (header.type == ArchiveType::Encrypted) {
        for (auto& entry : file_entries) {
            uint32_t key_1 = entry.key;
            uint32_t key_2 = entry.span.key;

            decrypt(entry.key, entry.span.key, (uint32_t*)&entry);

            entry.key = key_1;
            entry.span.key = key_2;
        }

        for (auto& entry : chunk_entries) {
            uint32_t key_1 = entry.decompressed_span.key;
            uint32_t key_2 = entry.compressed_span.key;

            decrypt(entry.decompressed_span.key, entry.compressed_span.key, (uint32_t*)&entry);

            entry.decompressed_span.key = key_1;
            entry.compressed_span.key = key_2;
        }
    }

    for (std::size_t index = 0; index < file_entries.size(); index++) {
        m_hash_to_index.emplace(file_entries.at(index).hash, index);
    }

    return true;
}

Decima::OptionalRef<Decima::CoreFile> Decima::Archive::query_file(std::uint64_t hash) {
    if (auto index = m_hash_to_index.find(hash); index != m_hash_to_index.end()) {
        auto cache = m_cache.find(index->second);

        if (cache == m_cache.end()) {
            Decima::CoreFile file(*this, file_entries.at(index->second), m_stream);
            cache = m_cache.emplace(index->second, std::move(file)).first;
        }

        return std::make_optional(std::ref(cache->second));
    }

    return {};
}

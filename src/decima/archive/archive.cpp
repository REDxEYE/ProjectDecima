#include "decima/archive/archive.hpp"
#include "decima/shared.hpp"

#include <fstream>
#include <MurmurHash3.h>

static void decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data) {
    const std::uint32_t key[8] = {
        key_1, Decima::plain_cipher_key[1], Decima::plain_cipher_key[2], Decima::plain_cipher_key[3],
        key_2, Decima::plain_cipher_key[1], Decima::plain_cipher_key[2], Decima::plain_cipher_key[3]
    };

    std::uint32_t iv[8];
    MurmurHash3_x64_128(key, 16, Decima::cipher_seed, iv);
    MurmurHash3_x64_128(key + 4, 16, Decima::cipher_seed, iv + 4);

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
    : m_file(std::make_unique<std::ifstream>(path, std::ios::binary))
    , path(path) { open(); }

bool Decima::Archive::open() {
    m_file->seekg(0, std::ios::beg);
    m_file->read((char*)&header, sizeof(ArchiveHeader));

    if (header.type != ArchiveType::Regular && header.type != ArchiveType::Encrypted)
        return false;

    if (header.type == ArchiveType::Encrypted)
        decrypt(header.key, header.key + 1, (uint32_t*)&header.file_size);

    file_entries.resize(header.file_entries_count);
    m_file->read((char*)file_entries.data(), sizeof(ArchiveFileEntry) * header.file_entries_count);

    chunk_entries.resize(header.chunk_entries_count);
    m_file->read((char*)chunk_entries.data(), sizeof(ArchiveChunkEntry) * header.chunk_entries_count);

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

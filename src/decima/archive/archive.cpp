//
// Created by MED45 on 25.07.2020.
//

#include <iostream>
#include <filesystem>

#include "decima/archive/archive.h"
#include "utils.h"

#include <MurmurHash3.h>
#include <md5.h>

Decima::Archive::Archive(const std::string& workdir, const std::string& filename)
    : filepath(workdir + "\\" + filename) { }

Decima::Archive::Archive(const std::string& workdir, uint64_t filehash)
    : Archive(workdir, uint64_to_hex(filehash) + ".bin") { }

bool Decima::Archive::open() {
    std::error_code error;
    filebuffer.map(filepath, error);

    if (error)
        return false;

    memcpy(&header, filebuffer.data(), sizeof(ArchiveHeader));

    if (!is_valid())
        return false;

    if (is_encrypted())
        decrypt(header.key, header.key + 1, (uint32_t*)&header + 2);

    std::size_t read_offset = sizeof(header);

    content_table.resize(header.content_table_size);
    memcpy(content_table.data(), filebuffer.data() + read_offset,
        sizeof(FileEntry) * header.content_table_size);

    read_offset += sizeof(FileEntry) * header.content_table_size;

    chunk_table.resize(header.chunk_table_size);
    memcpy(chunk_table.data(), filebuffer.data() + read_offset, sizeof(chunk_table.front()) * header.chunk_table_size);

    if (is_encrypted()) {
        for (auto& file_entry : content_table) {
            decrypt(file_entry.key, file_entry.key2, (uint32_t*)&file_entry);
        }

        for (auto& chunk : chunk_table) {
            auto saved_key = chunk.key_1;
            decrypt(chunk.key_1, chunk.key_2, (uint32_t*)&chunk);
            chunk.key_1 = saved_key;
        }
    }

    return true;
}

bool Decima::Archive::is_valid() const {
    return (header.version == Decima::Version::default_version || header.version == Decima::Version::encrypted_version);
}

bool Decima::Archive::is_encrypted() const {
    return header.version == Decima::Version::encrypted_version;
}

void Decima::Archive::decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data) {
    const std::uint32_t key[8] = {
        key_1, encryption_key_1[1], encryption_key_1[2], encryption_key_1[3],
        key_2, encryption_key_1[1], encryption_key_1[2], encryption_key_1[3]
    };

    std::uint32_t iv[8];
    MurmurHash3_x64_128(key, 16, seed, iv);
    MurmurHash3_x64_128(key + 4, 16, seed, iv + 4);

    data[0] ^= iv[0];
    data[1] ^= iv[1];
    data[2] ^= iv[2];
    data[3] ^= iv[3];
    data[4] ^= iv[4];
    data[5] ^= iv[5];
    data[6] ^= iv[6];
    data[7] ^= iv[7];
}



[[maybe_unused]] uint64_t Decima::Archive::get_file_index(const std::string& file_name) const {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return get_file_index(hash);
}

uint64_t Decima::Archive::get_file_index(uint64_t file_hash) const {
    for (std::size_t i = 0; i < content_table.size(); i++) {
        if (content_table[i].hash == file_hash)
            return i;
    }

    return -1;
}

uint64_t Decima::Archive::chunk_id_by_offset(uint64_t offset) {
    for (std::size_t i = 0; i < chunk_table.size(); i++) {
        if (chunk_table[i].uncompressed_offset == offset)
            return i;
    }

    return -1;
}

Decima::CoreFile Decima::Archive::query_file(uint64_t file_hash) {
    //    log("Archive", "Queried " + uint64_to_hex(file_hash) + " file");
    auto file_id = get_file_index(file_hash);
    if (file_id == -1) {
        return Decima::CoreFile(nullptr, nullptr, nullptr, true);
    }
    auto& file_entry = content_table.at(file_id);
    Decima::CoreFile file(&file_entry, &filebuffer, this, is_encrypted());

    file.chunk_range = get_mio_boundaries(file_id);

    return file;
}

[[maybe_unused]] Decima::CoreFile Decima::Archive::query_file(const std::string& file_name) {
    log("Archive", "Queried " + file_name + " file");
    return query_file(hash_string(sanitize_name(file_name), seed));
}

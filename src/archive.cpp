//
// Created by MED45 on 25.07.2020.
//

#include <iostream>
#include <filesystem>

#include "archive.h"
#include "utils.h"

#include <MurmurHash3.h>
#include <md5.h>

template <class T>
using slice = std::pair<std::decay_t<T>, std::decay_t<T>>;

Decima::Archive::Archive(const std::string& workdir, const std::string& filename)
    : filepath(workdir + "\\" + filename) { }

Decima::Archive::Archive(const std::string& workdir, uint64_t filehash)
    : Archive(workdir, uint64_to_hex(filehash) + ".bin") { }

bool Decima::Archive::open() {
    std::error_code error;
    filebuffer.map(filepath, error);

    if (error)
        return false;

    memcpy(&header, filebuffer.data(), sizeof(structs::ArchiveHeader));

    if (!is_valid())
        return false;

    if (is_encrypted())
        decrypt(header.key, header.key + 1, (uint32_t*)&header + 2);

    std::size_t read_offset = sizeof(header);

    content_table.resize(header.content_table_size);
    memcpy(content_table.data(), filebuffer.data() + read_offset, sizeof(structs::FileEntry) * header.content_table_size);

    read_offset += sizeof(structs::FileEntry) * header.content_table_size;

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
    const uint32_t inputKey[2][4] = {
        { key_1, encryption_key_1[1], encryption_key_1[2], encryption_key_1[3] },
        { key_2, encryption_key_1[1], encryption_key_1[2], encryption_key_1[3] }
    };

    uint32_t iv[4];

    for (int i = 0; i < 2; i++) {
        MurmurHash3_x64_128(inputKey[i], 0x10, seed, iv);
        for (int j = 0; j < 4; j++) {
            data[(i * 4) + j] ^= iv[j];
        }
    }
}

std::vector<uint8_t> Decima::Archive::extract_file_data(int32_t file_id) {
    if (file_id == -1)
        return {};

    const auto& file_entry = content_table.at(file_id);

    const auto file_offset = file_entry.offset;
    const auto file_size = file_entry.size;

    const auto first_chunk = calculate_first_containing_chunk(file_offset, header.max_chunk_size);
    const auto last_chunk = calculate_last_containing_chunk(file_offset, file_size, header.max_chunk_size);

    const auto first_chunk_row = find_chunk_by_offset(first_chunk);
    const auto last_chunk_row = find_chunk_by_offset(last_chunk);
    const auto max_needed_size = (last_chunk_row - first_chunk_row + 1) * header.max_chunk_size;

    std::vector<uint8_t> out_data(max_needed_size);

    for (std::uint64_t i = first_chunk_row, pos = 0; i <= last_chunk_row; i++) {
        const auto& chunk = chunk_table.at(i);
        std::vector<uint8_t> chunk_data = get_chunk_data(chunk);

        if (is_encrypted())
            decrypt_chunk(i, chunk_data);

        decompress_chunk_data(chunk_data, chunk.uncompressed_size, &out_data.at(pos));
        pos += chunk.uncompressed_size;
    }

    const auto file_position = file_offset % header.max_chunk_size;

    out_data.erase(out_data.begin(), out_data.begin() + file_position);
    out_data.erase(out_data.begin() + file_size, out_data.begin() + out_data.size());

    return out_data;
}

std::vector<uint8_t> Decima::Archive::get_chunk_data(const Decima::structs::ChunkEntry& chunk) {
    const auto chunk_offset = chunk.compressed_offset;
    const auto chunk_size = chunk.compressed_size;
    std::vector<uint8_t> data;

    data.resize(chunk_size);
    memcpy(data.data(), filebuffer.data() + chunk_offset, chunk_size);

    return data;
}

void Decima::Archive::decrypt_chunk(uint32_t chunk_id, std::vector<uint8_t>& src) {
    uint32_t iv[4];
    MurmurHash3_x64_128(&chunk_table[chunk_id].uncompressed_offset, 0x10, seed, iv);

    for (int i = 0; i < 4; i++) {
        iv[i] ^= encryption_key_2[i];
    }

    uint8_t digest[16];
    md5Hash((md5_byte_t*)iv, 16, digest);
    for (int i = 0; i < src.size(); i++) {
        src[i] ^= digest[i % 16];
    }
}

uint64_t Decima::Archive::get_file_index(const std::string& file_name) const {
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

uint64_t Decima::Archive::find_chunk_by_offset(uint64_t offset) {
    for (std::size_t i = 0; i < chunk_table.size(); i++) {
        if (chunk_table[i].uncompressed_offset == offset)
            return i;
    }

    return -1;
}

std::vector<uint8_t> Decima::Archive::query_file(uint32_t file_hash) {
    return extract_file_data(get_file_index(file_hash));
}

std::vector<uint8_t> Decima::Archive::query_file(const std::string& file_name) {
    return extract_file_data(get_file_index(file_name));
}

//
// Created by MED45 on 25.07.2020.
//

#include <iostream>
#include <filesystem>
#include "decima_archive.h"
#include "utils.h"
#include "MurmurHash3.h"
#include <windows.h>
#include <md5.h>

Decima::Archive::Archive(const std::string& workdir, const std::string& filename) {
    filepath = workdir + "\\" + filename;
    filebuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);

}

Decima::Archive::Archive(const std::string& workdir, uint64_t filehash) : Archive(workdir, uint64_to_hex(filehash) +
                                                                                           ".bin") {}

bool Decima::Archive::open() {
    filebuffer.open(filepath, std::ios::binary);

    if (filebuffer.is_open()) {
        filebuffer.read((char*) &header, sizeof(header));
        if (is_valid()) {
            if (is_encrypted()) {
                decrypt(header.key, header.key + 1, ((uint32_t*) &header) + 2);
            }
            return true;
        }
    }
    return false;
}

bool Decima::Archive::is_valid() const {
    return (header.magic == magic || header.magic == encrypted_magic);
}

bool Decima::Archive::is_encrypted() const {
    return header.magic == encrypted_magic;
}


void Decima::Archive::decrypt(uint32_t key_1, uint32_t key_2, uint32_t* data) {
    uint32_t iv[4];
    uint32_t inputKey[2][4] = {
            {key_1, murmur_salt[1], murmur_salt[2], murmur_salt[3]},
            {key_2, murmur_salt[1], murmur_salt[2], murmur_salt[3]}
    };

    for (int i = 0; i < 2; i++) {
        MurmurHash3_x64_128(inputKey[i], 0x10, seed, iv);
        for (int j = 0; j < 4; j++) {
            data[(i * 4) + j] ^= iv[j];
        }
    }
}


void Decima::Archive::read_content_table() {
    content_table.resize(header.content_table_size);
    file_hashes.resize(header.content_table_size);
    bool encrypted = is_encrypted();
    for (uint64_t i = 0; i < header.content_table_size; i++) {
        filebuffer.read((char*) &content_table.at(i), sizeof(Decima::structs::FileEntry));
        auto& file_entry = content_table.at(i);
        if (encrypted) decrypt(file_entry.key, file_entry.key2, (uint32_t*) &file_entry);
        file_hashes.push_back(file_entry.hash);
    }
}

void Decima::Archive::read_chunk_table() {
    chunk_table.resize(header.chunk_table_size);
    bool encrypted = is_encrypted();
    for (uint64_t i = 0; i < header.chunk_table_size; i++) {
        filebuffer.read((char*) &chunk_table.at(i), sizeof(Decima::structs::ChunkEntry));
        auto& chunk = chunk_table.at(i);
        auto saved_key = chunk.key_1;
        if (encrypted) decrypt(chunk.key_1, chunk.key_2, (uint32_t*) &chunk);
        chunk.key_1 = saved_key;
    }
}

std::vector<uint8_t> Decima::Archive::extract_file_data(int32_t file_id) {
    if (file_id == -1)return {};
    auto& file_entry = content_table.at(file_id);
    uint64_t file_offset = file_entry.offset;
    uint32_t file_size = file_entry.size;

    uint64_t first_chunk = calculate_first_containing_chunk(file_offset, header.max_chunk_size);
    uint64_t last_chunk = calculate_last_containing_chunk(file_offset, file_size, header.max_chunk_size);

    uint64_t first_chunk_row = find_chunk_by_offset(first_chunk);
    uint64_t last_chunk_row = find_chunk_by_offset(last_chunk);
    uint64_t max_needed_size = ((last_chunk_row - first_chunk_row) + 1) * header.max_chunk_size;

    std::vector<uint8_t> out_data(max_needed_size);

    uint64_t pos = 0;
    for (uint32_t i = first_chunk_row; i <= last_chunk_row; i++) {
        auto& chunk = chunk_table.at(i);
        std::vector<uint8_t> chunk_data;
        get_chunk_data(chunk, chunk_data);
        if (is_encrypted())decrypt_chunk(i, chunk_data);
        decompress_chunk_data(chunk_data, chunk.uncompressed_size, &out_data.at(pos));
        pos += chunk.uncompressed_size;
    }
    uint64_t file_position = file_offset % header.max_chunk_size;
    out_data.erase(out_data.begin(),out_data.begin()+file_position);
    out_data.erase(out_data.begin()+file_size,out_data.begin()+out_data.size());
    return std::move(out_data);

}

//void Decima::Archive::get_file_data(const std::string& file_name, std::vector<uint8_t>& data_out) {
//    uint64_t id = get_file_index(file_name);
//    if (id != -1) {
//        extract_file_data(id, data_out);
//    }
//}

void Decima::Archive::get_chunk_data(Decima::structs::ChunkEntry& chunk, std::vector<uint8_t>& data) {
    uint64_t chunk_offset = chunk.compressed_offset;
    uint64_t chunk_size = chunk.compressed_size;

    data.resize(chunk_size);
    filebuffer.seekg(chunk_offset);
    filebuffer.read(reinterpret_cast<char*>(data.data()), chunk_size);

}

void Decima::Archive::decrypt_chunk(uint32_t chunk_id, std::vector<uint8_t>& src) {
    uint32_t iv[4];
    MurmurHash3_x64_128(&chunk_table[chunk_id].uncompressed_offset, 0x10, seed, iv);

    for (int i = 0; i < 4; i++) {
        iv[i] ^= murmur_salt2[i];
    }

    uint8_t digest[16];
    md5Hash((md5_byte_t*) iv, 16, digest);
    for (int i = 0; i < src.size(); i++) {
        src[i] ^= digest[i % 16];
    }
}

uint64_t Decima::Archive::get_file_index(const std::string& file_name) const {
    uint64_t hash = hash_string(sanitize_name(file_name), seed);
    return get_file_index(hash);
}

uint64_t Decima::Archive::get_file_index(uint64_t file_hash) const {
    for (uint64_t i = 0; i < content_table.size(); i++) {
        if (content_table[i].hash == file_hash)return i;
    }
    return -1;
}

uint64_t Decima::Archive::find_chunk_by_offset(uint64_t offset) {
    for (int i = 0; i < chunk_table.size(); i++) {
        if (chunk_table[i].uncompressed_offset == offset)
            return i;
    }
    return -1;
}

std::vector<uint8_t> Decima::Archive::query_file(uint32_t file_hash) {
    return std::move(extract_file_data(get_file_index(file_hash)));
}

std::vector<uint8_t> Decima::Archive::query_file(const std::string& file_name) {
    return std::move(extract_file_data(get_file_index(file_name)));
}


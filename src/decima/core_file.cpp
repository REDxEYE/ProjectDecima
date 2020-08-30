//
// Created by i.getsman on 04.08.2020.
//

#include "decima/core_file.hpp"

#include <md5.h>
#include <MurmurHash3.h>

#include "decima/archive/archive.hpp"
#include "decima/file_types.hpp"
#include "utils.hpp"

void Decima::CoreFile::unpack() {

    const auto [chunk_entry_begin, chunk_entry_end] = get_chunk_boundaries();

    std::size_t uncompressed_size = 0;

    for (auto chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        uncompressed_size += chunk_entry->uncompressed_size;
    }

    storage.clear();
    storage.resize(uncompressed_size);

    std::size_t buffer_offset = 0;

    for (auto chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        std::vector<std::uint8_t> data_buffer(chunk_entry->compressed_size);
        memcpy(data_buffer.data(), file_buffer.data() + chunk_entry->compressed_offset, chunk_entry->compressed_size);

        if (encrypted)
            decrypt_chunk(data_buffer.data(), *chunk_entry);

        decompress_chunk_data((std::uint8_t*)data_buffer.data(), chunk_entry->compressed_size, chunk_entry->uncompressed_size, (std::uint8_t*)&storage.at(buffer_offset));
        buffer_offset += chunk_entry->uncompressed_size;
    }

    const auto file_position = file_entry->offset % archive->content_info.max_chunk_size;
    storage.erase(storage.begin(), storage.begin() + file_position);
    storage.erase(storage.begin() + file_entry->size, storage.begin() + storage.size());
}

Decima::CoreFile::CoreFile(FileEntry* file_entry, ash::buffer file_buffer, Archive* archive, bool encrypted)
    : file_entry(file_entry)
    , file_buffer(file_buffer)
    , archive(archive)
    , encrypted(encrypted) { }

std::pair<std::vector<Decima::ChunkEntry>::iterator, std::vector<Decima::ChunkEntry>::iterator>
Decima::CoreFile::get_chunk_boundaries() {
    auto& chunk_table = archive->chunk_table;

    const auto file_offset = file_entry->offset;
    const auto file_size = file_entry->size;

    const auto first_chunk = calculate_first_containing_chunk(file_offset, archive->content_info.max_chunk_size);
    const auto last_chunk = calculate_last_containing_chunk(file_offset, file_size, archive->content_info.max_chunk_size);

    const auto first_chunk_row = chunk_id_by_offset(first_chunk);
    const auto last_chunk_row = chunk_id_by_offset(last_chunk);

    return { chunk_table.begin() + first_chunk_row, chunk_table.begin() + last_chunk_row + 1 };
}

std::uint64_t Decima::CoreFile::chunk_id_by_offset(uint64_t offset) const {
    for (std::size_t i = 0; i < archive->chunk_table.size(); i++) {
        if (archive->chunk_table[i].uncompressed_offset == offset)
            return i;
    }
    return -1;
}

void Decima::CoreFile::decrypt_chunk(uint8_t* data, const Decima::ChunkEntry& chunk_entry) {
    uint32_t iv[4];
    MurmurHash3_x64_128(&chunk_entry, 0x10, Decima::seed, iv);

    iv[0] ^= Decima::encryption_key_2[0];
    iv[1] ^= Decima::encryption_key_2[1];
    iv[2] ^= Decima::encryption_key_2[2];
    iv[3] ^= Decima::encryption_key_2[3];

    uint8_t digest[16];
    md5Hash((md5_byte_t*)iv, 16, digest);
    for (int i = 0; i < chunk_entry.compressed_size; i++) {
        data[i] ^= digest[i % 16];
    }
}

void Decima::CoreFile::parse(ArchiveArray& archive_array) {
    unpack();
    entries.clear();

    ash::buffer buffer(storage.data(), storage.size());

    for (std::size_t offset = 0; offset < buffer.size();) {
        const auto entry_header = Decima::CoreEntry::peek_header(buffer);

        entries.push_back([&] {
            auto handler = get_handler(entry_header.file_type);
            handler->parse(archive_array, buffer, this);
            handler->offset = offset;

            return handler;
        }());

        offset += entry_header.file_size + sizeof(Decima::GUID);
    }
}
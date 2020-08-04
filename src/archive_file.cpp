//
// Created by i.getsman on 04.08.2020.
//

#include "archive_file.h"
#include "archive.h"
#include "utils.h"

#include <md5.h>
#include "MurmurHash3.h"

void Decima::CompressedFile::unpack(uint32_t size) {
    uint32_t chunk_count = std::max((uint64_t)1, ((uint64_t)chunk_range.second - (uint64_t)chunk_range.first) / sizeof(ChunkEntry))+1;
    uint64_t required_size = 0;
    for (uint32_t i = 0; i < chunk_count; i++) {
        required_size += chunk_range.first[i].uncompressed_size;
    }
    auto data_in = decrypt(size);
    storage.clear();
    storage.resize(required_size);
    uint64_t in_pos = 0;
    uint64_t out_pos = 0;
    const auto file_position = file_entry->offset % archive->header.max_chunk_size;
    for (uint32_t chunk_id = 0; chunk_id < chunk_count; chunk_id++) {
        ChunkEntry* chunk = &chunk_range.first[chunk_id];
        decompress_chunk_data(data_in.data() + in_pos,
            chunk->compressed_size,
            chunk->uncompressed_size,
            &storage.at(out_pos));
        out_pos += chunk->uncompressed_size;
        in_pos += chunk->compressed_size;
    }
    storage.erase(storage.begin(), storage.begin() + file_position);
    storage.erase(storage.begin() + file_entry->size, storage.begin() + storage.size());
}

std::vector<uint8_t> Decima::CompressedFile::decrypt(uint32_t size) {
    uint32_t chunk_count = std::max((uint64_t)1, ((uint64_t)chunk_range.second - (uint64_t)chunk_range.first) / sizeof(ChunkEntry));
    uint64_t required_size = 0;
    for (uint32_t i = 0; i < chunk_count; i++) {
        required_size += chunk_range.first[i].compressed_size;
    }
    std::vector<std::uint8_t> output(required_size);
    uint64_t out_pos = 0;
    for (uint32_t chunk_id = 0; chunk_id < chunk_count; chunk_id++) {
        ChunkEntry* chunk = &chunk_range.first[chunk_id];
        uint32_t iv[4];
        MurmurHash3_x64_128(&chunk->uncompressed_offset, 0x10, seed, iv);

        for (int i = 0; i < 4; i++) {
            iv[i] ^= encryption_key_2[i];
        }

        uint8_t digest[16];
        md5Hash((md5_byte_t*)iv, 16, digest);
        for (int i = 0; i < chunk->compressed_size; i++) {
            output[out_pos + i] = (filebuffer->data() + chunk->compressed_offset)[i] ^ digest[i % 16];
        }
        out_pos += chunk->compressed_size;
    }
    return std::move(output);
}

Decima::CompressedFile::CompressedFile(FileEntry* file_entry_, mio::mmap_source* filebuffer_, Archive* archive_) {
    file_entry = file_entry_;
    filebuffer = filebuffer_;
    archive = archive_;
}

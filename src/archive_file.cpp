//
// Created by i.getsman on 04.08.2020.
//

#include "archive_file.h"
#include "archive.h"
#include "utils.h"

#include <md5.h>
#include "MurmurHash3.h"

void Decima::CompressedFile::unpack(uint32_t size) {
    uint64_t required_size = 0;
    auto&[chunk_entry_begin, chunk_entry_end] = chunk_range;
    for (auto& chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        required_size += chunk_entry->uncompressed_size;
    }
    auto data_in = decrypt(size);
    storage.clear();
    storage.resize(required_size);
    uint64_t in_pos = 0;
    uint64_t out_pos = 0;
    const auto file_position = file_entry->offset % archive->header.max_chunk_size;
    for (auto& chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        decompress_chunk_data(data_in.data() + in_pos,
                              chunk_entry->compressed_size,
                              chunk_entry->uncompressed_size,
                              &storage.at(out_pos));
        out_pos += chunk_entry->uncompressed_size;
        in_pos += chunk_entry->compressed_size;
    }
    storage.erase(storage.begin(), storage.begin() + file_position);
    storage.erase(storage.begin() + file_entry->size, storage.begin() + storage.size());
}

std::vector<uint8_t> Decima::CompressedFile::decrypt(uint32_t size) {
    uint64_t required_size = 0;
    auto&[chunk_entry_begin, chunk_entry_end] = chunk_range;
    for (auto& chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        required_size += chunk_entry->compressed_size;
    }
    std::vector<std::uint8_t> output(required_size);
    uint64_t out_pos = 0;
    for (auto& chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        uint32_t iv[4];
        MurmurHash3_x64_128(&chunk_entry->uncompressed_offset, 0x10, seed, iv);

        for (int i = 0; i < 4; i++) {
            iv[i] ^= encryption_key_2[i];
        }

        uint8_t digest[16];
        md5Hash((md5_byte_t*) iv, 16, digest);
        for (int i = 0; i < chunk_entry->compressed_size; i++) {
            output[out_pos + i] = (filebuffer->data() + chunk_entry->compressed_offset)[i] ^ digest[i % 16];
        }
        out_pos += chunk_entry->compressed_size;
    }
    return std::move(output);
}

Decima::CompressedFile::CompressedFile(FileEntry* file_entry_, mio::mmap_source* filebuffer_, Archive* archive_) {
    file_entry = file_entry_;
    filebuffer = filebuffer_;
    archive = archive_;
}

void Decima::CompressedFile::get_raw() {

}

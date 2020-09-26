#include "decima/archive/archive_file.hpp"

#include <numeric>
#include <md5.h>
#include <MurmurHash3.h>

#include "decima/archive/archive.hpp"
#include "decima/serializable/handlers.hpp"

static void decrypt_chunk(uint8_t* data, const Decima::ArchiveChunkEntry& chunk_entry) {
    uint32_t iv[4];
    MurmurHash3_x64_128(&chunk_entry, 0x10, Decima::seed, iv);

    iv[0] ^= Decima::encryption_key_2[0];
    iv[1] ^= Decima::encryption_key_2[1];
    iv[2] ^= Decima::encryption_key_2[2];
    iv[3] ^= Decima::encryption_key_2[3];

    uint8_t digest[16];
    md5Hash((md5_byte_t*)iv, 16, digest);
    for (uint32_t i = 0; i < chunk_entry.compressed_span.size; i++) {
        data[i] ^= digest[i % 16];
    }
}

std::vector<char> unpack(const Decima::Archive& archive, const Decima::ArchiveFileEntry& entry, mio::mmap_source& source) {
    auto aligned_offset = [](std::uint64_t offset, std::uint64_t alignment) {
        return offset & ~(alignment - 1);
    };

    auto chunk_from_offset = [&](std::uint64_t offset) {
        auto index = std::find_if(archive.chunk_entries.begin(), archive.chunk_entries.end(), [&](const auto& item) {
            return item.decompressed_span.offset == offset;
        });

        return std::distance(archive.chunk_entries.begin(), index);
    };

    auto [chunk_entry_begin, chunk_entry_end] = [&] {
        auto first_chunk_offset = aligned_offset(entry.span.offset, archive.header.chunk_maximum_size);
        auto last_chunk_offset = aligned_offset(entry.span.offset + entry.span.size, archive.header.chunk_maximum_size);

        return std::make_pair(
            archive.chunk_entries.begin() + chunk_from_offset(first_chunk_offset),
            archive.chunk_entries.begin() + chunk_from_offset(last_chunk_offset) + 1);
    }();

    auto decompressed_size = std::accumulate(chunk_entry_begin, chunk_entry_end, 0, [](const auto acc, const auto& item) {
        return acc + item.decompressed_span.size;
    });

    std::vector<char> buffer_decompressed(decompressed_size);
    std::size_t buffer_decompressed_offset = 0;

    for (auto chunk_entry = chunk_entry_begin; chunk_entry != chunk_entry_end; ++chunk_entry) {
        std::vector<std::uint8_t> data_buffer(chunk_entry->compressed_span.size);
        memcpy(data_buffer.data(), source.data() + chunk_entry->compressed_span.offset, chunk_entry->compressed_span.size);

        if (archive.header.type == Decima::ArchiveType::Encrypted)
            decrypt_chunk(data_buffer.data(), *chunk_entry);

        decompress_chunk_data((std::uint8_t*)data_buffer.data(), chunk_entry->compressed_span.size, chunk_entry->decompressed_span.size, (std::uint8_t*)&buffer_decompressed.at(buffer_decompressed_offset));
        buffer_decompressed_offset += chunk_entry->decompressed_span.size;
    }

    auto file_position = entry.span.offset & (archive.header.chunk_maximum_size - 1);
    buffer_decompressed.erase(buffer_decompressed.begin(), buffer_decompressed.begin() + file_position);
    buffer_decompressed.erase(buffer_decompressed.begin() + entry.span.size, buffer_decompressed.begin() + buffer_decompressed.size());

    return buffer_decompressed;
}

Decima::CoreFile::CoreFile(Archive& archive, ArchiveFileEntry& entry, mio::mmap_source& source)
    : archive(archive)
    , entry(entry)
    , contents(unpack(archive, entry, source)) { }

void Decima::CoreFile::parse(ArchiveManager& archive_array) {
    objects.clear();

    // TODO: This is shitty API I wrote, please replace this
    ash::buffer buffer(contents.data(), contents.size());

    while (buffer.size() > 0) {
        const auto entry_header = Decima::CoreObject::peek_header(buffer);
        const auto entry_offset = buffer.data() - contents.data();

        objects.push_back([&] {
            auto handler = Decima::get_type_handler(entry_header.file_type);
            handler->parse(archive_array, buffer, this);
            return std::make_pair(handler, entry_offset);
        }());
    }
}
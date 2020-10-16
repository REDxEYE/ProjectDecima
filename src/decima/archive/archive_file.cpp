#include "decima/archive/archive_file.hpp"

#include <algorithm>
#include <numeric>

#include <md5.h>
#include <MurmurHash3.h>

#include "decima/archive/archive.hpp"
#include "decima/archive/archive_manager.hpp"
#include "decima/serializable/object/object.hpp"
#include "decima/serializable/handlers.hpp"
#include "decima/serializable/reference.hpp"

static void decrypt_chunk(uint8_t* data, const Decima::ArchiveChunkEntry& chunk_entry) {
    uint32_t iv[4];
    MurmurHash3_x64_128(&chunk_entry, 0x10, Decima::cipher_seed, iv);

    iv[0] ^= Decima::chunk_cipher_key[0];
    iv[1] ^= Decima::chunk_cipher_key[1];
    iv[2] ^= Decima::chunk_cipher_key[2];
    iv[3] ^= Decima::chunk_cipher_key[3];

    uint8_t digest[16];
    md5Hash((md5_byte_t*)iv, 16, digest);
    for (uint32_t i = 0; i < chunk_entry.compressed_span.size; i++) {
        data[i] ^= digest[i % 16];
    }
}

std::vector<char> unpack(Decima::Compressor& compressor, const Decima::Archive& archive, const Decima::ArchiveFileEntry& entry, std::ifstream& source) {
    auto aligned_offset = [](std::uint64_t offset, std::uint64_t alignment) {
        return offset & ~(alignment - 1);
    };

    auto chunk_from_offset = [&](std::uint64_t offset) {
        return std::find_if(archive.chunk_entries.begin(), archive.chunk_entries.end(), [&](const auto& item) {
            return item.decompressed_span.offset == offset;
        });
    };

    auto [chunk_entry_begin, chunk_entry_end] = [&] {
        auto first_chunk_offset = aligned_offset(entry.span.offset, archive.header.chunk_maximum_size);
        auto last_chunk_offset = aligned_offset(entry.span.offset + entry.span.size, archive.header.chunk_maximum_size);

        return std::make_pair(chunk_from_offset(first_chunk_offset), chunk_from_offset(last_chunk_offset) + 1);
    }();

    source.seekg(chunk_entry_begin->compressed_span.offset, std::ios::beg);

    std::size_t chunk_buffer_offset = 0;
    std::vector<char> chunk_buffer;

    std::size_t result_buffer_size = 0;
    std::size_t result_buffer_offset = entry.span.offset & (archive.header.chunk_maximum_size - 1);
    std::vector<char> result_buffer;

    std::for_each(chunk_entry_begin, chunk_entry_end, [&] (const Decima::ArchiveChunkEntry& chunk) {
        chunk_buffer.resize(chunk_buffer.size() + chunk.compressed_span.size);
        source.read((char*)chunk_buffer.data() + chunk_buffer_offset, chunk.compressed_span.size);
        if (archive.header.type == Decima::ArchiveType::Encrypted)
            decrypt_chunk((uint8_t*)chunk_buffer.data() + chunk_buffer_offset, chunk);
        chunk_buffer_offset += chunk.compressed_span.size;
        result_buffer_size += chunk.decompressed_span.size;
    });

    result_buffer.resize(result_buffer_size);
    compressor.decompress(chunk_buffer, result_buffer);

    result_buffer.erase(result_buffer.begin(), result_buffer.begin() + result_buffer_offset);
    result_buffer.erase(result_buffer.begin() + entry.span.size, result_buffer.end());

    std::ofstream output("suka.bin", std::ios::binary | std::ios::trunc);
    output.write(result_buffer.data(), result_buffer.size());

    return result_buffer;
}

Decima::CoreFile::CoreFile(Archive& archive, ArchiveManager& manager, ArchiveFileEntry& entry, std::ifstream& source)
    : archive(archive)
    , manager(manager)
    , entry(entry)
    , contents(unpack(*manager.compressor, archive, entry, source)) { }

void Decima::CoreFile::resolve_reference(const std::shared_ptr<CoreObject>& object) {
    auto index = std::remove_if(references.begin(), references.end(), [&](Ref* ref) {
        if (ref->m_guid.hash() == object->guid.hash()) {
            ref->m_object = object;
            return true;
        }

        return false;
    });

    references.erase(index, references.end());
}

void Decima::CoreFile::resolve_reference(const Decima::CoreFile& file) {
    for (auto& [object, object_offset] : objects) {
        resolve_reference(object);
    }
}

void Decima::CoreFile::queue_reference(Decima::Ref* ref) {
    if (ref->mode() == RefLoadMode::NotPresent || ref->mode() == RefLoadMode::WorkOnly)
        return;

    if (ref->mode() == RefLoadMode::ImmediateCoreFile || ref->mode() == RefLoadMode::CoreFile) {
        auto& file = manager.query_file(ref->file().data()).value().get();
        file.references.push_back(ref);
        file.parse();
    } else {
        references.push_back(ref);
    }
}

void Decima::CoreFile::parse() {
    if (objects.empty()) {
        // TODO: This is shitty API I wrote, please replace this
        ash::buffer buffer(contents.data(), contents.size());

        while (buffer.size() > 0) {
            const auto entry_header = Decima::CoreObject::peek_header(buffer);
            const auto entry_offset = buffer.data() - contents.data();

            /*
             * Tricky hack to allow objects during parsing.
             * This is useful for getting owner of the reference
             */
            auto handler = Decima::get_type_handler(entry_header.file_type);
            objects.emplace_back(handler, entry_offset);
            handler->parse(manager, buffer, *this);
        }
    }

    resolve_reference(*this);
}

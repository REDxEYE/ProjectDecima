#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <fstream>

#include "decima/archive/archive_file.hpp"

namespace Decima {
    enum class ArchiveType : uint32_t {
        /** Regular type of the archive, not encrypted */
        Regular = 0x20304050,
        /** Regular type of the archive, encrypted with [key] */
        Encrypted = 0x21304050,
    };

    class ArchiveHeader {
    public:
        /** Type of this archive file, either regular or encrypted */
        ArchiveType type;
        /** Key used to decrypt contents of this archive */
        uint32_t key;
        /** Total size of this archive file, in bytes */
        uint64_t file_size;
        /** Total size of uncompressed files, in bytes */
        uint64_t data_size;
        /** Count of file entries */
        uint64_t file_entries_count;
        /** Count of chunk entries */
        uint32_t chunk_entries_count;
        /** Maximum size of chunk, in bytes */
        uint32_t chunk_maximum_size;
    };

    class ArchiveSpan {
    public:
        /** Starting offset of this span, in bytes */
        uint64_t offset;
        /** Size of this span, in bytes */
        uint32_t size;
        /** Key used to decrypt contents of this span */
        uint32_t key;
    };

    class ArchiveFileEntry {
    public:
        /** Unique identifier of this file entry */
        uint32_t index;
        /** Key used to decrypt contents of this file entry */
        uint32_t key;
        /** Hash of the name of this file entry, obtained using MurMurHash3 algorithm */
        uint64_t hash;
        /** Span of the decompressed data of this file entry */
        ArchiveSpan span;
    };

    class ArchiveChunkEntry {
    public:
        /** Span of the decompressed data of this chunk entry */
        ArchiveSpan decompressed_span;
        /** Span of the compressed data of this chunk entry */
        ArchiveSpan compressed_span;
    };

    class Archive {
    public:
        explicit Archive(const std::string& path);

        Decima::ArchiveHeader header {};
        std::vector<Decima::ArchiveFileEntry> file_entries;
        std::vector<Decima::ArchiveChunkEntry> chunk_entries;
        std::string path;

    private:
        friend class ArchiveManager;
        friend class CoreFile;

        bool open();

        std::unordered_map<std::uint64_t, Decima::CoreFile> m_cache;
        std::unordered_map<std::uint64_t, std::uint64_t> m_hash_to_index;
        std::unique_ptr<std::ifstream> m_file;
    };
}

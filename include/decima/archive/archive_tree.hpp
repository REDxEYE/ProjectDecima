#pragma once

#include <map>
#include <set>

#include <imgui.h>

#include "decima/archive/archive_file.hpp"
#include "decima/serializable/object/object.hpp"

struct FileInfo {
    std::string path;
    std::string name;
    std::uint64_t hash;

    inline operator bool() const {
        return hash != 0;
    }
};

struct SelectionInfo {
    SelectionInfo() = default;
    std::uint64_t preview_file { 0 };
    std::uint64_t preview_file_size { 0 };
    std::uint64_t preview_file_offset { 0 };
    std::uint64_t selected_file { 0 };
    FileInfo highlighted_file;
    std::set<std::uint64_t> selected_files;
    Decima::CoreFile* file;
};

template <class T>
using FileTreeToggleable = std::pair<T, bool>;

class FileTree {
public:
    enum class ExpandMode {
        None,
        Hide,
        Show
    };

    static constexpr std::size_t ExpandThreshold = 15;

    std::map<std::string, FileTreeToggleable<std::unique_ptr<FileTree>>> folders;
    std::map<std::string, FileTreeToggleable<FileInfo>> files;

    FileTree* add_folder(const std::string& name);
    void add_file(const std::string& filename, uint64_t hash, Decima::CoreHeader header);
    void add_file(const std::string& path, const std::string& filename, uint64_t hash, Decima::CoreHeader header);

    ExpandMode apply_filter(const ImGuiTextFilter& filter);
    void reset_filter(bool visibility);

    void draw(SelectionInfo& selection, Decima::ArchiveManager& manager, bool header, ExpandMode expand);

    std::size_t size() const;
};

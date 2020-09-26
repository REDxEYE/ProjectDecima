#pragma once

#include <unordered_set>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

#include "archive_manager.hpp"
#include "decima/serializable/object/object_dummy.hpp"

struct SelectionInfo {
    SelectionInfo() = default;
    std::uint64_t preview_file { 0 };
    std::uint64_t preview_file_size { 0 };
    std::uint64_t preview_file_offset { 0 };
    std::uint64_t selected_file { 0 };
    std::unordered_set<std::uint64_t> selected_files;
    Decima::CoreFile* file;
};

struct FileInfo {
    uint64_t hash { 0 };
    Decima::CoreHeader header { 0 };
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

    ExpandMode apply_filter(const ImGuiTextFilter& filter);
    void reset_filter(bool visibility);

    void draw(SelectionInfo& selection, Decima::ArchiveManager& archive_array, bool header, ExpandMode expand);

    std::size_t size() const;
};

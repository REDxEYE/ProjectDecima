//
// Created by MED45 on 27.07.2020.
//

#ifndef PROJECTDS_ARCHIVE_TREE_H
#define PROJECTDS_ARCHIVE_TREE_H

#include <unordered_set>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <imgui.h>

#include "archive_array.h"
#include "file_types/core.h"
#include "archive_file.h"

struct SelectionInfo {
    std::uint64_t preview_file { 0 };
    std::uint64_t selected_file { 0 };
    std::unordered_set<std::uint64_t> selected_files;
    Decima::CompressedFile file;
};

struct FileInfo {
    uint64_t hash { 0 };
    Decima::CoreHeader header { 0 };
};

struct FileTypeHandler {
    std::string name;
    std::function<void()> export_fn = nullptr;
    std::function<void(imemstream&)> render_fn = nullptr;
};

template <class T>
using FileTreeToggleable = std::pair<T, bool>;

class FileTree {
public:
    std::map<std::string, FileTreeToggleable<std::unique_ptr<FileTree>>> folders;
    std::map<std::string, FileTreeToggleable<FileInfo>> files;
    std::unordered_map<std::uint64_t, FileTypeHandler> file_type_handlers;

    FileTree* add_folder(const std::string& name);

    void add_file(const std::string& filename, uint64_t hash, Decima::CoreHeader header);

    void update_filter(const ImGuiTextFilter& filter);

    void reset_filter(bool state);

    template <typename Visitor>
    void visit(const Visitor& visitor, std::size_t depth = 0) const {
        for (const auto& [name, data] : folders) {
            visitor(name, depth);
            data.first->visit(visitor, depth + 1);
        }

        for (const auto& [name, _] : files) {
            visitor(name, depth);
        }
    }

    void draw(SelectionInfo& selection, Decima::ArchiveArray& archive_array);
};

#endif //PROJECTDS_ARCHIVE_TREE_H
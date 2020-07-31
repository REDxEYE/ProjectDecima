//
// Created by MED45 on 27.07.2020.
//

#ifndef PROJECTDS_FILE_TREE_H
#define PROJECTDS_FILE_TREE_H

#include <imgui.h>
#include "archive_array.h"

#include "file_types/core.h"

#include <unordered_set>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct SelectionInfo {
    std::uint64_t preview_file{0};
    std::uint64_t selected_file{0};
    std::unordered_set<std::uint64_t> selected_files;
    std::vector<std::uint8_t> file_data;
};

struct FileInfo {
    uint64_t hash{0};
    Decima::CoreHeader header{0};
};

template<class T>
using FileTreeToggleable = std::pair<T, bool>;

class FileTree {
public:

    std::map<std::string, FileTreeToggleable<std::unique_ptr<FileTree>>> folders;
    std::map<std::string, FileTreeToggleable<FileInfo>> files;

    FileTree* add_folder(const std::string& name);

    void add_file(const std::string& filename, uint64_t hash, Decima::CoreHeader header);

    void update_filter(const ImGuiTextFilter& filter);

    void reset_filter(bool state);

    template<typename Visitor>
    void visit(const Visitor& visitor, std::size_t depth = 0) const {
        for (const auto&[name, data] : folders) {
            visitor(name, depth);
            data.first->visit(visitor, depth + 1);
        }

        for (const auto&[name, _] : files) {
            visitor(name, depth);
        }
    }

    void draw(SelectionInfo& selection, Decima::ArchiveArray& archive_array);
};


#endif //PROJECTDS_FILE_TREE_H

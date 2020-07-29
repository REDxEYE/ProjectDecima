//
// Created by MED45 on 27.07.2020.
//

#ifndef PROJECTDS_FILE_TREE_H
#define PROJECTDS_FILE_TREE_H

#include "archive_array.h"
#include <unordered_set>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

template<class T>
using FileTreeToggleable = std::pair<T, bool>;

class FileTree {
public:
    std::map<std::string, FileTreeToggleable<std::unique_ptr<FileTree>>> folders;
    std::map<std::string, FileTreeToggleable<uint32_t>> files;

    FileTree* add_folder(const std::string &name);

    void add_file(const std::string &filename, uint32_t hash);

    void update_filter(const ImGuiTextFilter &filter);

    void reset_filter(bool state);

    template <typename Visitor>
    void visit(const Visitor& visitor, std::size_t depth = 0) const {
        for (const auto& [name, data] : folders) {
            visitor(name, depth);
            data.first->visit(visitor, depth + 1);
        }

        for(const auto& [name, _] : files) {
            visitor(name, depth);
        }
    }

    void draw(std::unordered_set<uint32_t>& selected_files, std::uint32_t& current_selected_file, Decima::ArchiveArray &archive_array);
};


#endif //PROJECTDS_FILE_TREE_H

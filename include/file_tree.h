//
// Created by MED45 on 27.07.2020.
//

#ifndef PROJECTDS_FILE_TREE_H
#define PROJECTDS_FILE_TREE_H

#include "archive_array.h"
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

template <class T>
using FileTreeToggleable = std::pair<T, bool>;

class FileTree {
public:
    std::map<std::string, FileTreeToggleable<std::unique_ptr<FileTree>>> folders;
    std::map<std::string, FileTreeToggleable<uint32_t>> files;

    FileTree* add_folder(const std::string& name);

    void add_file(const std::string& filename, uint32_t hash);

    void update_filter(const ImGuiTextFilter& filter);

    void reset_filter(bool state);

    void draw(uint32_t& selected_file_hash, Decima::ArchiveArray &archive_array);
};


#endif //PROJECTDS_FILE_TREE_H

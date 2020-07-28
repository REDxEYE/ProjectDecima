//
// Created by MED45 on 27.07.2020.
//

#include "file_tree.h"
#include "imgui.h"
#include <filesystem>

FileTree* FileTree::add_folder(const std::string& name)
{
    if (folders.find(name) == folders.end())
        folders.emplace(name, std::make_pair(std::make_unique<FileTree>(), true));
    return folders.at(name).first.get();
}

void FileTree::add_file(const std::string& filename, uint32_t hash)
{
    files.emplace(filename, std::make_pair(hash, true));
}

bool is_filter_matches(FileTree* root, const ImGuiTextFilter& filter)
{
    bool result = false;

    for (auto& [name, data] : root->files) {
        if (filter.PassFilter(name.c_str())) {
            data.second = true;
            result = true;
        }
    }

    for (auto& [name, data] : root->folders) {
        if (is_filter_matches(data.first.get(), filter)) {
            data.second = true;
            result = true;
        }
    }

    return result;
}

void FileTree::update_filter(const ImGuiTextFilter& filter)
{
    if (filter.IsActive()) {
        reset_filter(false);
        is_filter_matches(this, filter);
    } else {
        reset_filter(true);
    }
}

void FileTree::reset_filter(bool state)
{
    for (auto& [_, data] : folders) {
        data.first->reset_filter(state);
        data.second = state;
    }

    for (auto& [_, data] : files) {
        data.second = state;
    }
}

void FileTree::draw(uint32_t& selected_file_hash)
{
    for (auto& [name, data] : folders) {
        if (data.second && ImGui::TreeNode(name.c_str())) {
            data.first->draw(selected_file_hash);
            ImGui::TreePop();
        }
    }

    for (auto& [name, data] : files) {
        if (data.second && ImGui::Selectable(name.c_str())) {
            selected_file_hash = data.first;
        }
    }
}

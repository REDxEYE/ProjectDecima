//
// Created by MED45 on 27.07.2020.
//

#include "file_tree.h"
#include "imgui.h"
#include "utils.h"
#include <filesystem>

FileTree* FileTree::add_folder(const std::string& name) {
    if (folders.find(name) == folders.end())
        folders.emplace(name, std::make_pair(std::make_unique<FileTree>(), true));
    return folders.at(name).first.get();
}

void FileTree::add_file(const std::string& filename, uint32_t hash, Decima::CoreHeader header) {
    files.emplace(filename, std::make_pair(FileInfo{hash, header}, true));
}

bool is_filter_matches(FileTree* root, const ImGuiTextFilter& filter) {
    bool result = false;

    for (auto&[name, data] : root->files) {
        if (filter.PassFilter(name.c_str())) {
            data.second = true;
            result = true;
        }
    }

    for (auto&[name, data] : root->folders) {
        if (is_filter_matches(data.first.get(), filter)) {
            data.second = true;
            result = true;
        }
    }

    return result;
}

void FileTree::update_filter(const ImGuiTextFilter& filter) {
    if (filter.IsActive()) {
        reset_filter(false);
        is_filter_matches(this, filter);
    } else {
        reset_filter(true);
    }
}

void FileTree::reset_filter(bool state) {
    for (auto&[_, data] : folders) {
        data.first->reset_filter(state);
        data.second = state;
    }

    for (auto&[_, data] : files) {
        data.second = state;
    }
}

void FileTree::draw(SelectionInfo& selection, Decima::ArchiveArray& archive_array) {
    for (auto&[name, data] : folders) {
        const std::string tree_name = name + "##" + std::to_string(folders.size());
        const auto show = ImGui::TreeNode(tree_name.c_str());
        const auto files_count = data.first->files.size();
        const auto folders_count = data.first->folders.size();

        ImGui::NextColumn();
        ImGui::Text("Folder");
        ImGui::NextColumn();
        ImGui::Text("%llu file%c / %llu folder%c",
                    files_count, files_count == 1 ? ' ' : 's',
                    folders_count, folders_count == 1 ? ' ' : 's');
        ImGui::NextColumn();

        if (data.second && show) {
            data.first->draw(selection, archive_array);
            ImGui::TreePop();
        }
    }

    for (auto&[name, data] : files) {
        if (!data.second)
            continue;

        auto is_selected = selection.selected_files.find(data.first.hash) != selection.selected_files.end();
        if (ImGui::Selectable(name.c_str(), is_selected)) {
            selection.selected_file = data.first.hash;

            if (ImGui::GetIO().KeyCtrl) {
                if (is_selected) {
                    selection.selected_files.erase(data.first.hash);
                } else {
                    selection.selected_files.insert(data.first.hash);
                }
            }
        }

        auto filename = sanitize_name(archive_array.hash_to_name[data.first.hash]);
        auto file_entry = archive_array.get_file_entry(filename);
        auto size = file_entry->size;

        ImGui::NextColumn();
        auto& file_info = data.first.header;
        if (Decima::known_file_types.find(file_info.filetype) != Decima::known_file_types.end()) {
            ImGui::Text("%s",Decima::known_file_types.at(file_info.filetype).c_str());
        } else {
            ImGui::Text("Unknown");
        }
        ImGui::NextColumn();
        ImGui::Text("%dB", size);
        ImGui::NextColumn();
    }
}

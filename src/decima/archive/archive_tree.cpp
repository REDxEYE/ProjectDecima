//
// Created by MED45 on 27.07.2020.
//

#include <filesystem>

#include "decima/archive/archive_tree.h"

#include <imgui.h>
#include "utils.h"

FileTree* FileTree::add_folder(const std::string& name) {
    if (folders.find(name) == folders.end())
        folders.emplace(name, std::make_pair(std::make_unique<FileTree>(), true));
    return folders.at(name).first.get();
}

void FileTree::add_file(const std::string& filename, uint64_t hash, Decima::CoreHeader header) {
    files.emplace(filename, std::make_pair(FileInfo { hash, header }, true));
}

bool is_filter_matches(FileTree* root, const ImGuiTextFilter& filter) {
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

void FileTree::update_filter(const ImGuiTextFilter& filter) {
    if (filter.IsActive()) {
        reset_filter(false);
        is_filter_matches(this, filter);
    } else {
        reset_filter(true);
    }
}

void FileTree::reset_filter(bool state) {
    for (auto& [_, data] : folders) {
        data.first->reset_filter(state);
        data.second = state;
    }

    for (auto& [_, data] : files) {
        data.second = state;
    }
}

void FileTree::draw(SelectionInfo& selection, Decima::ArchiveArray& archive_array, bool draw_header) {
    if (draw_header) {
        ImGui::Separator();
        ImGui::Columns(3);
        ImGui::Text("Name");
        ImGui::NextColumn();
        ImGui::Text("Type");
        ImGui::NextColumn();
        ImGui::Text("Size");
        ImGui::NextColumn();
        ImGui::Separator();

        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 200);
        ImGui::SetColumnWidth(1, 100);
        ImGui::SetColumnWidth(2, 100);
    }

    for (auto& [name, data] : folders) {
        if (!data.second)
            continue;

        const auto tree_name = name + "##" + std::to_string(folders.size());
        const auto show = ImGui::TreeNodeEx(tree_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);
        const auto items_count = data.first->files.size() + data.first->folders.size();

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen() && ImGui::GetIO().KeyCtrl) {
            /*
             * Check whether all files in this folders are selected or not,
             * if selected, then deselect everything, if at least one is not selected,
             * then select everything
             */

            std::unordered_set<std::uint64_t> folder_files;

            for (const auto& file : data.first->files) {
                folder_files.insert(file.second.first.hash);
            }

            bool contains_all = true;

            for (const auto& item : folder_files) {
                if (selection.selected_files.find(item) == selection.selected_files.end()) {
                    contains_all = false;
                    break;
                }
            }

            if (contains_all) {
                for (const auto& item : folder_files)
                    selection.selected_files.erase(item);
            } else {
                for (const auto& item : folder_files)
                    selection.selected_files.insert(item);
            }
        }

        ImGui::NextColumn();
        ImGui::Text("Folder");
        ImGui::NextColumn();
        ImGui::Text("%llu item%c", items_count, items_count == 1 ? ' ' : 's');
        ImGui::NextColumn();

        if (show) {
            if (items_count > 0) {
                data.first->draw(selection, archive_array, false);
            } else {
                ImGui::TextDisabled("Empty");
            }
            ImGui::TreePop();
        }
    }

    for (auto& [name, data] : files) {
        if (!data.second)
            continue;

        const auto is_selected = selection.selected_files.find(data.first.hash) != selection.selected_files.end();

        ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf | is_selected);

        if(ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            selection.selected_file = data.first.hash;

            if (ImGui::GetIO().KeyCtrl) {
                if (is_selected) {
                    selection.selected_files.erase(data.first.hash);
                } else {
                    selection.selected_files.insert(data.first.hash);
                }
            }
        }

        ImGui::TreePop();

        ImGui::NextColumn();
        ImGui::Text("File");
        ImGui::NextColumn();

        const auto file_entry = archive_array.get_file_entry(data.first.hash);

        if (file_entry.has_value()) {
            ImGui::Text("%s", format_size(file_entry.value().get().size).c_str());
        } else {
            ImGui::Text("Unknown");
        }

        ImGui::NextColumn();
    }

    if (draw_header) {
        ImGui::Columns(1);
    }
}

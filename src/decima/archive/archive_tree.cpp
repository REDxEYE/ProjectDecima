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
        const auto tree_name = name + "##" + std::to_string(folders.size());
        const auto show = ImGui::TreeNode(tree_name.c_str());
        const auto items_count = data.first->files.size() + data.first->folders.size();

        ImGui::NextColumn();
        ImGui::Text("Folder");
        ImGui::NextColumn();
        ImGui::Text("%llu item%c", items_count, items_count == 1 ? ' ' : 's');
        ImGui::NextColumn();

        if (data.second && show) {
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

        ImGui::NextColumn();
        ImGui::Text("File");
        ImGui::NextColumn();

        const auto file_entry = archive_array.get_file_entry(data.first.hash);

        if (file_entry.has_value()) {
            ImGui::Text("%dB", file_entry.value().get().size);
        } else {
            ImGui::Text("Unknown");
        }

        ImGui::NextColumn();
    }

    if (draw_header) {
        ImGui::Columns(1);
    }
}

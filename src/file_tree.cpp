//
// Created by MED45 on 27.07.2020.
//

#include "file_tree.h"
#include "imgui.h"
#include <filesystem>
#include <utils.h>

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

void FileTree::draw(uint32_t& selected_file_hash, Decima::ArchiveArray &archive_array)
{
    for (auto& [name, data] : folders) {
        const auto show = ImGui::TreeNode(name.c_str());
        const auto size = data.first->files.size();

        ImGui::NextColumn();
        ImGui::Text("Folder");
        ImGui::NextColumn();
        ImGui::Text("%llu file%c", size, size == 1 ? ' ' : 's');
        ImGui::NextColumn();

        if (data.second && show) {
            data.first->draw(selected_file_hash, archive_array);
            ImGui::TreePop();
        }
    }

    for (auto& [name, data] : files) {
        if (!data.second)
            continue;

        if (ImGui::Selectable(name.c_str()))
            selected_file_hash = data.first;

        auto filename = sanitize_name(archive_array.hash_to_name[data.first]);
        auto* file_entry = archive_array.get_file_entry(filename);
        auto size = file_entry->size;

        ImGui::NextColumn();
        ImGui::Text("Unknown");
        ImGui::NextColumn();
        ImGui::Text("%dB", size);
        ImGui::NextColumn();
    }
}

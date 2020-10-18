//
// Created by MED45 on 27.07.2020.
//

#include <filesystem>

#include "decima/archive/archive_tree.hpp"
#include "decima/archive/archive_manager.hpp"
#include "decima/archive/archive.hpp"

#include <imgui.h>
#include "utils.hpp"

FileTree* FileTree::add_folder(const std::string& name) {
    if (folders.find(name) == folders.end())
        folders.emplace(name, std::make_pair(std::make_unique<FileTree>(), true));
    return folders.at(name).first.get();
}

void FileTree::add_file(const std::string& filename, uint64_t hash, Decima::CoreHeader header) {
    add_file(filename, filename, hash, header);
}

void FileTree::add_file(const std::string& path, const std::string& filename, uint64_t hash, Decima::CoreHeader header) {
    FileInfo info {};
    info.path = path;
    info.name = filename;
    info.hash = hash;
    files.emplace(filename, std::make_pair(std::move(info), true));
}

bool is_filter_matches(FileTree& root, const ImGuiTextFilter& filter) {
    bool result = false;

    for (auto& [name, data] : root.files) {
        if (filter.PassFilter(name.c_str())) {
            data.second = true;
            result = true;
        }
    }

    for (auto& [name, data] : root.folders) {
        if (is_filter_matches(*data.first, filter)) {
            data.second = true;
            result = true;
        }
    }

    return result;
}

FileTree::ExpandMode FileTree::apply_filter(const ImGuiTextFilter& filter) {
    if (filter.IsActive()) {
        reset_filter(false);
        is_filter_matches(*this, filter);
    } else {
        reset_filter(true);
    }

    return size() < ExpandThreshold ? ExpandMode::Show : ExpandMode::Hide;
}

void FileTree::reset_filter(bool visibility) {
    for (auto& [_, data] : folders) {
        data.first->reset_filter(visibility);
        data.second = visibility;
    }

    for (auto& [_, data] : files) {
        data.second = visibility;
    }
}

void FileTree::draw(SelectionInfo& selection, Decima::ArchiveManager& manager, bool header, ExpandMode expand) {
    if (header) {
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

        if (expand != ExpandMode::None)
            ImGui::SetNextItemOpen(expand == ExpandMode::Show, ImGuiCond_Always);

        const auto tree_name = name + "##" + std::to_string(folders.size());
        const auto show = ImGui::TreeNodeEx(tree_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick);
        const auto items_count = data.first->files.size() + data.first->folders.size();

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen() && ImGui::GetIO().KeyCtrl) {
            /*
             * Check whether all files in this folders are selected or not,
             * if selected, then deselect everything, if at least one is not selected,
             * then select everything
             */

            std::set<std::uint64_t> folder_files;

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
                data.first->draw(selection, manager, false, expand);
            } else {
                ImGui::TextDisabled("Empty");
                ImGui::NextColumn();
                ImGui::NextColumn();
                ImGui::NextColumn();
            }
            ImGui::TreePop();
        }
    }

    for (auto& [name, data] : files) {
        if (!data.second)
            continue;

        if (expand != ExpandMode::None)
            ImGui::SetNextItemOpen(expand == ExpandMode::Show, ImGuiCond_Always);

        const auto is_selected = selection.selected_files.find(data.first.hash) != selection.selected_files.end();

        ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf | static_cast<int>(is_selected));

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
            selection.selected_file = data.first.hash;

            if (ImGui::GetIO().KeyCtrl) {
                if (is_selected) {
                    selection.selected_files.erase(data.first.hash);
                } else {
                    selection.selected_files.insert(data.first.hash);
                }
            }
        } else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
            selection.highlighted_file = data.first;
            ImGui::OpenPopup("TreeContextMenu");
        }

        ImGui::SetNextWindowSize({ 320, 0 }, ImGuiCond_Always);

        if (ImGui::BeginPopupContextWindow("TreeContextMenu")) {
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1, 1, 1, 0.5 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1, 1, 1, 0.4 });

            if (ImGui::Button("Copy name", { -1, 0 })) {
                ImGui::SetClipboardText(selection.highlighted_file.path.c_str());
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Copy hash", { -1, 0 })) {
                ImGui::SetClipboardText(uint64_to_hex(selection.highlighted_file.hash).c_str());
                ImGui::CloseCurrentPopup();
            }

            ImGui::Separator();

            if (ImGui::Button("Find out what references this file", { -1, 0 })) {
                DECIMA_LOG("What references file '", selection.highlighted_file.path, "':");

                auto index = manager.hash_to_index.at(selection.highlighted_file.hash);
                auto links = manager.prefetch->links.at(index).data();
                auto paths = manager.prefetch->paths.data();

                for (auto& link : links) {
                    DECIMA_LOG(" - '", paths.at(link).data(), "'");
                }

                if (links.empty()) {
                    DECIMA_LOG(" - <none>");
                }

                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Find out what is referenced by this file", { -1, 0 })) {
                DECIMA_LOG("What is referenced by file '", selection.highlighted_file.path, "':");

                auto index = manager.hash_to_index.at(selection.highlighted_file.hash);
                auto paths = manager.prefetch->paths.data();
                auto found = false;

                for (std::uint64_t link_index = 0; link_index < paths.size(); link_index++) {
                    for (auto& link : manager.prefetch->links.at(link_index).data()) {
                        if (link == index) {
                            DECIMA_LOG(" - '", paths.at(link_index).data(), "'");
                            found = true;
                        }
                    }
                }

                if (!found) {
                    DECIMA_LOG(" - <none>");
                }

                ImGui::CloseCurrentPopup();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
            ImGui::EndPopup();
        }

        ImGui::TreePop();

        ImGui::NextColumn();
        ImGui::Text("File");
        ImGui::NextColumn();

        const auto file_entry = manager.get_file_entry(data.first.hash);

        if (file_entry.has_value()) {
            ImGui::Text("%s", format_size(file_entry.value().get().span.size).c_str());
        } else {
            ImGui::Text("Unknown");
        }

        ImGui::NextColumn();
    }

    if (header) {
        ImGui::Columns(1);
    }
}

std::size_t FileTree::size() const {
    std::size_t size = 0;

    for (auto& [name, data] : files) {
        if (data.second)
            size += 1;
    }

    for (auto& [name, data] : folders) {
        size += data.first.get()->size();
    }

    return size;
}

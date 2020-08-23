//
// Created by i.getsman on 06.08.2020.
//

#include <filesystem>

#include "projectds_app.hpp"

#include "utils.h"
#include "portable-file-dialogs.h"

#include "decima/file_types/core/dummy.h"

static void show_data_selection_dialog(ProjectDS& self) {
    auto folder = pfd::select_folder("Select Death Stranding data folder!").result();

    if (!folder.empty()) {
        self.archive_array.open(folder);
        self.file_names.clear();
        self.file_names.reserve(self.archive_array.hash_to_name.size());

        for (const auto& [hash, path] : self.archive_array.hash_to_name) {
            self.file_names.push_back(path.c_str());

            std::vector<std::string> split_path;
            split(path, split_path, '/');

            auto* current_root = &self.root_tree;

            for (auto it = split_path.begin(); it != split_path.end() - 1; it++)
                current_root = current_root->add_folder(*it);

            if (self.archive_array.hash_to_archive.find(hash) != self.archive_array.hash_to_archive.end())
                current_root->add_file(split_path.back(), hash, { 0 });
        }
    }
}

static void show_export_selection_dialog(ProjectDS& self) {
    if (self.selection_info.selected_files.empty())
        return;

    const auto base_folder = pfd::select_folder("Choose destination folder").result();

    if (!base_folder.empty()) {
        for (const auto selected_file : self.selection_info.selected_files) {
            const auto filename = sanitize_name(self.archive_array.hash_to_name.at(selected_file));

            std::filesystem::path full_path = std::filesystem::path(base_folder) / filename;
            std::filesystem::create_directories(full_path.parent_path());

            auto file = self.archive_array.query_file(filename);
            file.unpack();
            std::ofstream output_file { full_path, std::ios::binary };
            output_file.write(reinterpret_cast<const char*>(file.storage.data()), file.storage.size());

            std::cout << "File was exported to: " << full_path << "\n";
        }
    }
}

void ProjectDS::input_user() {
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureKeyboard)
        return;

    if (io.KeyCtrl) {
        if (io.KeysDown[GLFW_KEY_O])
            show_data_selection_dialog(*this);

        if (io.KeysDown[GLFW_KEY_E])
            show_export_selection_dialog(*this);

        if (io.KeysDown[GLFW_KEY_A])
            current_popup = io.KeyShift ? Popup::AppendExportByHash : Popup::AppendExportByName;
    }

    if (io.KeysDown[GLFW_KEY_ESCAPE]) {
        if (selection_info.preview_file != 0) {
            selection_info.preview_file_size = selection_info.file.storage.size();
            selection_info.preview_file_offset = 0;
        }
    }
}

void ProjectDS::update_user(double ts) {
    App::update_user(ts);
    draw_dockspace();
    draw_filepreview();
    draw_export();
    draw_tree();
}

void ProjectDS::init_filetype_handlers() {
}

void ProjectDS::draw_dockspace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    const auto dock_flags = ImGuiWindowFlags_MenuBar
        | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoBackground;
    ImGui::Begin("DockSpace", nullptr, dock_flags);
    {
        ImGui::PopStyleVar(3);
        ImGui::DockSpace(ImGui::GetID("Dock"));

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open archive", "Ctrl+O")) {
                    show_data_selection_dialog(*this);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}

void ProjectDS::draw_filepreview() {
    ImGui::Begin("File preview");
    {
        if (selection_info.selected_file > 0) {
            const auto file_entry_opt = archive_array.get_file_entry(selection_info.selected_file);

            if (file_entry_opt.has_value()) {
                const auto& file_entry = file_entry_opt.value().get();

                std::string filename;
                if (archive_array.hash_to_name.find(selection_info.selected_file) != archive_array.hash_to_name.end()) {
                    filename = sanitize_name(archive_array.hash_to_name.at(selection_info.selected_file));
                } else {
                    filename = uint64_to_hex(selection_info.selected_file);
                }
                ImGui::TextWrapped("%s", filename.c_str());

                if (ImGui::BeginPopupContextItem("File preview name")) {
                    if (ImGui::Selectable("Copy path"))
                        ImGui::SetClipboardText(filename.c_str());
                    ImGui::EndPopup();
                }

                ImGui::Separator();
                ImGui::Columns(2);
                ImGui::Text("Size");
                ImGui::NextColumn();
                ImGui::Text("%u bytes", file_entry.size);
                ImGui::Separator();
                ImGui::NextColumn();
                ImGui::Text("Hash");
                ImGui::NextColumn();
                ImGui::Text("%llX", file_entry.hash);
                ImGui::Separator();
                ImGui::NextColumn();
                ImGui::Text("Entry ID");
                ImGui::NextColumn();
                ImGui::Text("%u", file_entry.entry_num);
                ImGui::Separator();
                ImGui::NextColumn();
                ImGui::Text("Offset");
                ImGui::NextColumn();
                ImGui::Text("%llu", file_entry.offset);
                ImGui::Columns(1);
                ImGui::Separator();

                const bool selected_file_changed = selection_info.preview_file != selection_info.selected_file;

                if (selected_file_changed) {
                    selection_info.file = archive_array.query_file(selection_info.selected_file);
                    selection_info.file.unpack();
                    selection_info.preview_file = selection_info.selected_file;
                    selection_info.preview_file_size = selection_info.file.storage.size();
                    selection_info.preview_file_offset = 0;

                    parse_core_file();
                }
            } else {
                ImGui::Text("Error getting file info!");
            }
        } else {
            ImGui::Text("No file selected");
        }

        ImGui::DockSpace(ImGui::GetID("Dock2"));
    }
    ImGui::End();

    ImGui::Begin("Normal View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (selection_info.selected_file > 0) {
            for (const auto& file : parsed_files) {
                std::stringstream buffer;
                buffer << '[' << file->guid << "] " << Decima::get_type_name(file->header.file_type);

                const bool opened = ImGui::TreeNode(buffer.str().c_str());

                if (ImGui::BeginPopupContextItem(buffer.str().c_str())) {
                    if (ImGui::Selectable("Highlight")) {
                        selection_info.preview_file_offset = file->offset + sizeof(Decima::CoreHeader) + sizeof(Decima::GUID);
                        selection_info.preview_file_size = file->header.file_size - sizeof(Decima::GUID);
                    }

                    ImGui::EndPopup();
                }

                if (opened) {
                    file->draw();
                    ImGui::TreePop();
                }

                ImGui::Separator();
            }
        }
    }
    ImGui::End();

    ImGui::Begin("Raw View", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (selection_info.selected_file > 0) {
            file_viewer.DrawContents(
                selection_info.file.storage.data() + selection_info.preview_file_offset,
                selection_info.preview_file_size,
                0);
        }
    }
    ImGui::End();
}

void ProjectDS::draw_tree() {
    ImGui::Begin("Trees");
    {
        if (filter.Draw()) {
            root_tree.update_filter(filter);

            file_names.clear();

            for (auto& [_, path] : archive_array.hash_to_name) {
                if (filter.PassFilter(path.c_str())) {
                    file_names.push_back(path.c_str());
                }
            }
        }

        ImGui::BeginChild("FileTree");
        root_tree.draw(selection_info, archive_array);
        ImGui::EndChild();
    }
    ImGui::End();
}

void ProjectDS::draw_export() {
    ImGui::Begin("Export");
    {
        if (ImGui::Button("Dump all files` paths as a tree")) {
            const auto full_path = pfd::save_file("Choose destination file").result();

            if (!full_path.empty()) {
                std::ofstream output_file { full_path };

                root_tree.visit([&](const auto& name, auto depth) {
                    output_file << std::string(depth * 2, ' ');
                    output_file << name;
                    output_file << '\n';
                });

                std::cout << "File was saved to: " << full_path << '\n';
            }
        }

        if (ImGui::Button("Dump all files` full names")) {
            const auto full_path = pfd::save_file("Choose destination file").result();

            if (!full_path.empty()) {
                std::ofstream output_file { full_path };

                for (auto& [hash, path] : archive_array.hash_to_name) {
                    auto file = archive_array.query_file(hash);

                    if (file.file_entry) {
                        file.unpack();
                        Decima::Source stream(file.storage, 1024);

                        std::cout << "Processing file '" << path << "' (size: " << file.file_entry->size << ")\n";

                        while (!stream.eof()) {
                            Decima::Dummy dummy;
                            dummy.parse(archive_array, stream);

                            output_file << path << '_' << dummy.guid << '\n';
                        }
                    }
                }

                std::cout << "File was saved to: " << full_path << '\n';
            }
        }

        if (ImGui::Button("Dump all files` hashes")) {
            const auto full_path = pfd::save_file("Choose destination file").result();

            if (!full_path.empty()) {
                std::ofstream output_file { full_path };

                for (const auto& archive : archive_array.archives) {
                    output_file << archive.filepath << '\n';

                    for (const auto& entry : archive.content_table) {
                        const auto name = archive_array.hash_to_name.find(entry.hash);

                        if (name != archive_array.hash_to_name.end())
                            output_file << "  name: '" << name->second << "'\n";

                        output_file << "  hash: '" << entry.hash << "'\n";
                        output_file << "  size: '" << entry.size << "'\n";
                        output_file << '\n';
                    }
                }

                std::cout << "File was saved to: " << full_path << '\n';
            }
        }

        if (ImGui::Button("Add file by name"))
            current_popup = Popup::AppendExportByName;

        if (ImGui::Button("Add file by hash"))
            current_popup = Popup::AppendExportByHash;

        if (current_popup == Popup::AppendExportByName) {
            ImGui::OpenPopup("AppendExportByName");
            current_popup = Popup::None;
        }

        if (ImGui::BeginPopup("AppendExportByName")) {
            static char path[512];

            const auto submit = ImGui::InputText("File name", path, IM_ARRAYSIZE(path), ImGuiInputTextFlags_EnterReturnsTrue);

            if (submit || ImGui::Button("Add to selection!")) {
                std::string str_path(path);
                uint64_t file_hash = hash_string(sanitize_name(str_path), Decima::seed);
                if (archive_array.get_file_entry(file_hash).has_value()) {
                    archive_array.hash_to_name[file_hash] = str_path;
                    selection_info.selected_files.insert(file_hash);
                }
            }

            ImGui::EndPopup();
        }

        if (current_popup == Popup::AppendExportByHash) {
            ImGui::OpenPopup("AppendExportByHash");
            current_popup = Popup::None;
        }

        if (ImGui::BeginPopup("AppendExportByHash")) {
            static uint64_t file_hash;

            const auto submit = ImGui::InputScalar("File hash", ImGuiDataType_U64, &file_hash, nullptr, nullptr, nullptr, ImGuiInputTextFlags_EnterReturnsTrue);

            if (submit || ImGui::Button("Add to selection!")) {
                if (archive_array.get_file_entry(file_hash).has_value()) {
                    archive_array.hash_to_name[file_hash] = "HASH: " + uint64_to_hex(file_hash);
                    selection_info.selected_files.insert(file_hash);
                }
            }

            ImGui::EndPopup();
        }

        if (ImGui::ListBoxHeader("Selected files")) {
            for (const auto selected_file : selection_info.selected_files) {
                if (archive_array.hash_to_name.find(selected_file) != archive_array.hash_to_name.end()) {
                    if (ImGui::Selectable(archive_array.hash_to_name[selected_file].c_str()))
                        selection_info.selected_file = selected_file;
                } else {
                    std::string new_name = "Hash: " + uint64_to_hex(selected_file);
                    if (ImGui::Selectable(new_name.c_str()))
                        selection_info.selected_file = selected_file;
                }
            }

            ImGui::ListBoxFooter();
        }

        if (ImGui::Button("Export selected file(-s)")) {
            show_export_selection_dialog(*this);
        }

        if (selection_info.selected_files.empty()) {
            ImGui::SameLine();
            ImGui::Text("No files selected");
        }
    }
    ImGui::End();
}

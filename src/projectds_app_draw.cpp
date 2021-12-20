//
// Created by i.getsman on 06.08.2020.
//

#include <filesystem>
#include <fstream>

#include "projectds_app.hpp"

#include "decima/serializable/handlers.hpp"
#include "util/pfd.h"
#include "utils.hpp"

static void show_data_selection_dialog(ProjectDS& self) {
    std::string folder = pfd::select_folder("Select game folder").result();
    std::string compressor_file;

    if (!folder.empty()) {
        for (auto file : std::filesystem::recursive_directory_iterator(folder)) {
            auto filename = file.path().filename();

            if (filename.extension() == ".dll" && filename.string().find("oo2core") == 0 && compressor_file.empty()) {
                compressor_file = file.path().string();
            }

            if (filename.extension() == ".bin") {
                DECIMA_LOG("Loading archive '", file.path().stem().string(), "'");
                self.archive_manager.load_archive(file.path().string());
            }
        }

        if (compressor_file.empty()) {
            DECIMA_LOG("Could not find compressor library");

            while (true) {
                auto result = pfd::open_file("Select oo2core_X_win64.dll", "", { "oo2core_X_win64.dll", "oo2core_*_win64.dll" }).result();

                if (!result.empty()) {
                    compressor_file = result[0];
                    break;
                }
            }
        }

        self.archive_manager.compressor = std::make_unique<Decima::Compressor>(compressor_file);

        DECIMA_LOG("Using compressor '", std::filesystem::path(compressor_file).filename().string(), "' (version ", self.archive_manager.compressor->get_version_string(), ")");

        if (self.archive_manager.compressor->get_version() < 0x2E070030) {
            pfd::message("Unsupported compressor", "Compressor library version must be at least 2.7.0 (oo2core_7_win64) or greater", pfd::choice::ok, pfd::icon::error);
            std::exit(EXIT_FAILURE);
        }

        self.archive_manager.load_prefetch();

        self.file_names.clear();
        self.file_names.reserve(self.archive_manager.hash_to_name.size());

        std::thread([](ProjectDS& self) {
            self.root_tree_constructing = true;

            for (const auto& [hash, path] : self.archive_manager.hash_to_name) {
                self.file_names.push_back(path.c_str());

                std::vector<std::string> split_path;
                split(path, split_path, '/');

                auto* current_root = &self.root_tree;

                for (auto it = split_path.begin(); it != split_path.end() - 1; it++)
                    current_root = current_root->add_folder(*it);

                if (self.archive_manager.hash_to_archive_index.find(hash) != self.archive_manager.hash_to_archive_index.end()) {
                    current_root->add_file(path, split_path.back(), hash, { 0 });
                }
            }

            self.root_tree_constructing = false;
        }, std::ref(self)).detach();
    }
}

static void show_export_selection_dialog(ProjectDS& self) {
    const auto write_to_file = [](Decima::CoreFile& file, const std::filesystem::path& path) {
        std::filesystem::create_directories(path.parent_path());

        std::ofstream output_file{ path, std::ios::binary };
        output_file.write(reinterpret_cast<const char*>(file.contents.data()), file.contents.size());

        std::cout << "File was exported to: " << path << "\n";
    };

    if (self.selection_info.selected_files.empty())
        return;

    if (const auto root = pfd::select_folder("Choose destination folder").result(); !root.empty()) {
        for (const auto selected_file : self.selection_info.selected_files) {
            if (auto entry = self.archive_manager.hash_to_name.find(selected_file); entry != self.archive_manager.hash_to_name.end()) {
                write_to_file(self.archive_manager.query_file(entry->second).value(), std::filesystem::path(root) / entry->second);
            } else {
                write_to_file(self.archive_manager.query_file(selected_file).value(), std::filesystem::path(root) / uint64_to_hex(selected_file));
            }
        }
    }
}

void ProjectDS::init_user() {
    App::init_user();
    init_imgui();
    init_filetype_handlers();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    file_viewer.WriteFn = [](auto, auto, auto) {
        /* Dummy write function because
         * ReadOnly forbids any selection. */
    };

    shortcuts.push_back(ShortcutInfo {
        "Escape",
        "Reset highlighted selection in raw view",
        GLFW_KEY_ESCAPE,
        ImGuiKeyModFlags_None,
        [&] {
            if (selection_info.preview_file != 0) {
                selection_info.preview_file_size = selection_info.file->contents.size();
                selection_info.preview_file_offset = 0;
            }
        },
    });

    shortcuts.push_back(ShortcutInfo {
        "Ctrl+O",
        "Open file dialog to select game folder",
        GLFW_KEY_O,
        ImGuiKeyModFlags_Ctrl,
        [&] { show_data_selection_dialog(*this); },
    });

    shortcuts.push_back(ShortcutInfo {
        "Ctrl+E",
        "Export currently selected files",
        GLFW_KEY_E,
        ImGuiKeyModFlags_Ctrl,
        [&] { show_export_selection_dialog(*this); },
    });

    shortcuts.push_back(ShortcutInfo {
        "Ctrl+A",
        "Add file to selection by its name",
        GLFW_KEY_A,
        ImGuiKeyModFlags_Ctrl,
        [&] { current_popup = Popup::AppendExportByName; },
    });

    shortcuts.push_back(ShortcutInfo {
        "Ctrl+Shift+A",
        "Add file to selection by its hash",
        GLFW_KEY_A,
        ImGuiKeyModFlags_Ctrl | ImGuiKeyModFlags_Shift,
        [&] { current_popup = Popup::AppendExportByHash; },
    });
}

void ProjectDS::input_user() {
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureKeyboard)
        return;

    for (const auto& shortcut : shortcuts) {
        if ((io.KeyMods & shortcut.mods) == shortcut.mods && io.KeysDown[shortcut.key])
            shortcut.callback();
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

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About"))
                    current_popup = Popup::About;

                if (ImGui::MenuItem("Shortcuts"))
                    current_popup = Popup::Shortcuts;

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }
    ImGui::End();

    if (current_popup == Popup::Shortcuts) {
        ImGui::OpenPopup("Shortcuts");
        current_popup = Popup::None;
    }

    if (current_popup == Popup::About) {
        ImGui::OpenPopup("About");
        current_popup = Popup::None;
    }

    ImGui::SetNextWindowSize({ 500, 0 }, ImGuiCond_Always);

    if (ImGui::BeginPopupModal("Shortcuts", nullptr, ImGuiWindowFlags_NoMove)) {
        ImGui::Columns(2);
        {
            ImGui::Text("Name");
            ImGui::SetColumnWidth(-1, 150);
            ImGui::NextColumn();

            ImGui::Text("Description");
            ImGui::NextColumn();

            ImGui::Separator();

            for (const auto& shortcut : shortcuts) {
                ImGui::TextUnformatted(shortcut.name.data());
                ImGui::NextColumn();

                ImGui::TextWrapped("%s", shortcut.description.data());
                ImGui::NextColumn();

                ImGui::Separator();
            }
        }
        ImGui::Columns(1);

        if (ImGui::Button("Got it", { -1, 0 }))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::SetNextWindowSize({ 600, 0 }, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, { 0.5, 0.5 });

    if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_NoMove)) {
        ImGui::Text("ProjectDecima " DECIMA_VERSION " (" __DATE__ ")");
        ImGui::Separator();

        ImGui::TextWrapped("This project is aimed to provide GUI for export/preview files inside Decima engine manager.");

        if (ImGui::TreeNode("Show license")) {
            ImGui::Text(R"(MIT License

Copyright (c) 2020 REDxEYE & ShadelessFox

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)");
            ImGui::TreePop();
        }

        ImGui::Separator();

        if (ImGui::Button("Source code", { -1, 0 }))
            std::system("start https://github.com/REDxEYE/ProjectDecima");

        if (ImGui::Button("Report problem", { -1, 0 }))
            std::system("start https://github.com/REDxEYE/ProjectDecima/issues");

        ImGui::Separator();

        if (ImGui::Button("Close", { -1, 0 }))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void ProjectDS::draw_filepreview() {
    ImGui::Begin("File preview");
    {
        if (selection_info.selected_file > 0) {
            const auto file_entry_opt = archive_manager.get_file_entry(selection_info.selected_file);

            if (file_entry_opt.has_value()) {
                const auto& file_entry = file_entry_opt.value().get();

                std::string filename;
                if (archive_manager.hash_to_name.find(selection_info.selected_file) != archive_manager.hash_to_name.end()) {
                    filename = sanitize_name(archive_manager.hash_to_name.at(selection_info.selected_file));
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
                {
                    ImGui::Text("Archive ID");
                    ImGui::NextColumn();

                    ImGui::Text("%s", archive_manager.archives.at(archive_manager.hash_to_archive_index.at(selection_info.selected_file)).path.c_str());
                    ImGui::NextColumn();

                    ImGui::Separator();

                    ImGui::Text("Size");
                    ImGui::NextColumn();

                    ImGui::Text("%u bytes", file_entry.span.size);
                    ImGui::NextColumn();

                    ImGui::Separator();

                    ImGui::Text("Hash");
                    ImGui::NextColumn();

                    ImGui::Text("%llX", file_entry.hash);
                    ImGui::NextColumn();

                    ImGui::Separator();

                    ImGui::Text("Entry ID");
                    ImGui::NextColumn();

                    ImGui::Text("%u", file_entry.index);
                    ImGui::NextColumn();

                    ImGui::Separator();

                    ImGui::Text("Offset");
                    ImGui::NextColumn();

                    ImGui::Text("%llu", file_entry.span.offset);
                    ImGui::Separator();
                }
                ImGui::Columns(1);

                const bool selected_file_changed = selection_info.preview_file != selection_info.selected_file;

                if (selected_file_changed) {
                    selection_info.file = &archive_manager.query_file(selection_info.selected_file).value().get();
                    selection_info.file->parse();
                    selection_info.preview_file = selection_info.selected_file;
                    selection_info.preview_file_size = selection_info.file->contents.size();
                    selection_info.preview_file_offset = 0;
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
            for (const auto& [file, file_offset] : selection_info.file->objects) {
                std::stringstream buffer;
                buffer << '[' << Decima::to_string(file->guid) << "] " << Decima::get_type_name(file->header.file_type);

                const bool opened = ImGui::TreeNode(buffer.str().c_str());

                if (ImGui::BeginPopupContextItem(buffer.str().c_str())) {
                    if (ImGui::Selectable("Highlight")) {
                        selection_info.preview_file_offset = file_offset;
                        selection_info.preview_file_size = file->header.file_size + sizeof(Decima::CoreHeader);
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
                selection_info.file->contents.data() + selection_info.preview_file_offset,
                selection_info.preview_file_size,
                0);
        }
    }
    ImGui::End();
}

void ProjectDS::draw_tree() {
    ImGui::Begin("Trees");
    {
        FileTree::ExpandMode expand_mode = FileTree::ExpandMode::None;

        if (filter.Draw()) {
            expand_mode = root_tree.apply_filter(filter);

            file_names.clear();

            for (auto& [_, path] : archive_manager.hash_to_name) {
                if (filter.PassFilter(path.c_str())) {
                    file_names.push_back(path.c_str());
                }
            }
        }

        if (root_tree_constructing) {
            ImGui::PushStyleColor(ImGuiCol_Text, 0xff99ffff);
            ImGui::TextWrapped("File tree is still constructing, some files may be missing");
            ImGui::PopStyleColor();
        }

        ImGui::BeginChild("FileTree");
        root_tree.draw(selection_info, archive_manager, true, expand_mode);
        ImGui::EndChild();
    }
    ImGui::End();
}

void ProjectDS::draw_export() {
    ImGui::Begin("Export");
    {
        if (ImGui::Button("Add file by name", { -1, 0 }))
            current_popup = Popup::AppendExportByName;

        if (ImGui::Button("Add file by hash", { -1, 0 }))
            current_popup = Popup::AppendExportByHash;

        if (ImGui::Button("Export selected items", { -1, 0 }))
            show_export_selection_dialog(*this);

        if (ImGui::PushItemWidth(-1), ImGui::ListBoxHeader("##", { 0, -1 })) {
            for (const auto selected_file : selection_info.selected_files) {
                if (archive_manager.hash_to_name.find(selected_file) != archive_manager.hash_to_name.end()) {
                    if (ImGui::Selectable(archive_manager.hash_to_name[selected_file].c_str()))
                        selection_info.selected_file = selected_file;
                } else {
                    std::string new_name = "Hash: " + uint64_to_hex(selected_file);
                    if (ImGui::Selectable(new_name.c_str()))
                        selection_info.selected_file = selected_file;
                }
            }

            ImGui::ListBoxFooter();
        }

        if (current_popup == Popup::AppendExportByName) {
            ImGui::OpenPopup("AppendExportByName");
            current_popup = Popup::None;
        }

        if (current_popup == Popup::AppendExportByName) {
            ImGui::OpenPopup("AppendExportByName");
            current_popup = Popup::None;
        }

        if (ImGui::BeginPopup("AppendExportByName")) {
            static char path[512];

            ImGui::InputText("File name", path, IM_ARRAYSIZE(path) - 1);

            if (ImGui::Button("Add to selection!")) {
                uint64_t file_hash = hash_string(sanitize_name(path), Decima::cipher_seed);
                if (archive_manager.get_file_entry(file_hash).has_value()) {
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
            static std::uint64_t file_hash = 0;

            ImGui::InputScalar("File hash", ImGuiDataType_U64, &file_hash);

            if (ImGui::Button("Add to selection!")) {
                if (archive_manager.get_file_entry(file_hash).has_value()) {
                    selection_info.selected_files.insert(file_hash);
                }
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

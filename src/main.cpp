#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_PERLIN_IMPLEMENTATION

#include <stb_perlin.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>

#include <filesystem>
#include <iostream>

#include "decima_archive.h"
#include "archive_array.h"
#include "file_tree.h"
#include "imgui_memory_editor.h"

#include <application/application.hpp>
#include <utils.h>

#include "portable-file-dialogs.h"

class MainLayer : public omc::layer {
    Decima::ArchiveArray archive_array;
    std::vector<const char*> file_names;
    FileTree root_tree;
    SelectionInfo selection_info;
    int32_t file_id = 0;
    ImGuiTextFilter filter;
    MemoryEditor file_viewer;

public:
    explicit MainLayer(omc::application* app)
        : layer(app) {
        file_viewer.ReadOnly = true;
    }

    void on_attach() override {
        layer::on_attach();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    void on_detach() override {
        layer::on_detach();
    }

    void on_update(double ts) override {
        omc::opengl_backend::imgui_new_frame();

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
                        auto folder = pfd::select_folder("Select Death Stranding data folder!").result();

                        if (!folder.empty()) {
                            archive_array.open(folder);
                            file_names.clear();
                            file_names.reserve(archive_array.hash_to_name.size());

                            for (auto& [hash, path] : archive_array.hash_to_name) {
                                file_names.push_back(path.c_str());
                                auto* current_root = &root_tree;

                                std::vector<std::string> split_path;
                                split(path, split_path, '/');

                                for (auto it = split_path.cbegin(); it != split_path.end() - 1; it++)
                                    current_root = current_root->add_folder(*it);

                                if (archive_array.hash_to_archive.find(hash) != archive_array.hash_to_archive.end()) {
                                    //                                    std::vector<uint8_t> tmp_vector = archive_array.query_file(hash);
                                    Decima::CoreHeader header { 0 };
                                    //                                    if (!tmp_vector.empty()) memcpy(&header, tmp_vector.data(), sizeof(header));
                                    current_root->add_file(split_path.back(), hash, header);
                                    //                                    tmp_vector.clear();
                                }
                            }
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }
        ImGui::End();

        {
            ImGui::Begin("DEBUG");
            //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0x80,0xFF,0x80,0xFF));
            //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0x80,0x80,0xFF,0xFF));
            //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0xF0,0x80,0xFF,0xFF));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::Begin("Test");
            if (ImGui::Button("Dump tree of files")) {
                const auto full_path = pfd::save_file("Choose destination file").result();

                if (!full_path.empty()) {
                    std::ofstream output_file { full_path };

                    root_tree.visit([&](const auto& name, auto depth) {
                        output_file << std::string(depth * 2, ' ');
                        output_file << name;
                        output_file << '\n';
                    });

                    std::cout << "File was saved to: " << full_path << "\n";
                }
            }

            ImGui::Separator();

            if (ImGui::ListBoxHeader("Selected files")) {
                for (const auto selected_file : selection_info.selected_files) {
                    if (ImGui::Selectable(archive_array.hash_to_name[selected_file].c_str()))
                        selection_info.selected_file = selected_file;
                }

                ImGui::ListBoxFooter();
            }

            if (ImGui::Button("Export selected file(-s)") && !selection_info.selected_files.empty()) {
                const auto base_folder = pfd::select_folder("Choose destination folder").result();

                if (!base_folder.empty()) {
                    for (const auto selected_file : selection_info.selected_files) {
                        namespace fs = std::filesystem;

                        const auto filename = sanitize_name(archive_array.hash_to_name.at(selected_file));

                        fs::path full_path = fs::path(base_folder) / filename;
                        fs::create_directories(full_path.parent_path());

                        std::vector<std::uint8_t> file_data = archive_array.query_file(filename);

                        std::ofstream output_file { full_path, std::ios::trunc };
                        output_file.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());

                        std::cout << "File was exported to: " << full_path << "\n";
                    }
                }
            }

            if (selection_info.selected_files.empty()) {
                ImGui::SameLine();
                ImGui::Text("No files selected");
            }

            ImGui::End();

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

                ImGui::BeginTabBar("Tabs");
                {
                    if (ImGui::BeginTabItem("ListView")) {
                        ImGui::PushItemWidth(-1);
                        if (ImGui::ListBox("TREE", &file_id, file_names.data(), file_names.size(), 50))
                            selection_info.selected_file = hash_string(sanitize_name(file_names[file_id]),
                                Decima::seed);
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("TreeView")) {
                        ImGui::Columns(3);

                        ImGui::Separator();
                        ImGui::Text("Name");
                        ImGui::NextColumn();
                        ImGui::Text("Type");
                        ImGui::NextColumn();
                        ImGui::Text("Size");
                        ImGui::NextColumn();
                        ImGui::Separator();

                        root_tree.draw(selection_info, archive_array);

                        ImGui::Columns(1);

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::End();

            ImGui::Begin("File preview");
            {
                if (selection_info.selected_file > 0) {
                    const auto filename = sanitize_name(archive_array.hash_to_name.at(selection_info.selected_file));
                    const auto file_entry_opt = archive_array.get_file_entry(filename);

                    if (file_entry_opt.has_value()) {
                        const auto& file_entry = file_entry_opt.value().get();
                        ImGui::Text("%s", filename.c_str());

                        ImGui::Columns(2);
                        ImGui::Separator();
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
                        ImGui::Separator();
                        ImGui::NextColumn();
                        ImGui::Separator();
                        ImGui::Columns(1);

                        if (selection_info.preview_file != selection_info.selected_file) {
                            selection_info.preview_file = selection_info.selected_file;
                            selection_info.file_data = archive_array.query_file(filename);
                        }
                        file_viewer.DrawContents(selection_info.file_data.data(), selection_info.file_data.size());
                    } else {
                        ImGui::Text("Error getting file info!");
                    }
                } else {
                    ImGui::Text("No file selected");
                }
            }
            ImGui::End();
        }
        omc::opengl_backend::imgui_render_frame();
    }
};

int main() {
    auto app = std::make_shared<omc::application>("Death Standing explorer", 1280, 720, true);

    app->push_layer(std::make_shared<MainLayer>(app.get()));

    app->run();
}
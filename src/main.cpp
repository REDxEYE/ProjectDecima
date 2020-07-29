#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_PERLIN_IMPLEMENTATION

#include <stb_perlin.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>

#include <utility>
#include <iostream>
#include <filesystem>
#include <unordered_set>

#include "archive/decima_archive.h"
#include "archive_array.h"
#include "file_tree.h"
#include "imgui_memory_editor.h"

#include <application/application.hpp>
#include <application/opengl_backend.h>
#include <utils.h>

#include "portable-file-dialogs.h"


struct SelectInfo {
    std::uint32_t preview_file{0};
    std::uint32_t selected_file{0};
    std::unordered_set<std::uint32_t> selected_files;
    std::vector<std::uint8_t> file_data;
};

class MainLayer : public omc::layer {
    Decima::ArchiveArray archive_array;
    std::vector<const char*> file_names;
    FileTree root_tree;

    SelectInfo select_info;

    int32_t file_id = 0;
    ImGuiTextFilter filter;
    MemoryEditor file_viewer;

public:
    explicit MainLayer(omc::application* app)
            : layer(app) {};

    void on_attach() override {
        layer::on_attach();
    }

    void on_detach() override {
        layer::on_detach();
    }

    void on_update(double ts) override {
        omc::opengl_backend::imgui_new_frame();
        {
            ImGui::Begin("DEBUG");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::End();

            ImGui::Begin("Test");
            if (ImGui::Button("Open archives")) {
                auto folder = pfd::select_folder("Select Death Stranding data folder!").result();

                if (!folder.empty()) {
                    archive_array.open(folder);
                    archive_array.read_content_table();
                    archive_array.read_prefetch_file();
                    file_names.clear();
                    file_names.reserve(archive_array.hash_to_name.size());

                    for (auto&[hash, path] : archive_array.hash_to_name) {
                        file_names.push_back(path.c_str());
                        auto* current_root = &root_tree;

                        std::vector<std::string> split_path;
                        split(path, split_path, '/');

                        for (auto it = split_path.cbegin(); it != split_path.end() - 1; it++)
                            current_root = current_root->add_folder(*it);

                        if (archive_array.hash_to_archive.find(hash) != archive_array.hash_to_archive.end())
                            current_root->add_file(split_path.back(), hash);
                    }
                }
            }

            if (ImGui::Button("Dump tree of files")) {
                const auto full_path = pfd::save_file("Choose destination file").result();

                if (!full_path.empty()) {
                    std::ofstream output_file{full_path};

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
                for (const auto selected_file : select_info.selected_files) {
                    if (ImGui::Selectable(archive_array.hash_to_name[selected_file].c_str()))
                        select_info.selected_file = selected_file;
                }

                ImGui::ListBoxFooter();
            }

            if (ImGui::Button("Export selected file(-s)") && !select_info.selected_files.empty()) {
                const auto base_folder = pfd::select_folder("Choose destination folder").result();

                if (base_folder.empty()) {
                    for (const auto selected_file : select_info.selected_files) {
                        namespace fs = std::filesystem;

                        const auto filename = sanitize_name(archive_array.hash_to_name.at(selected_file));

                        fs::path full_path = fs::path(base_folder) / filename;
                        fs::create_directories(full_path.parent_path());

                        std::vector<std::uint8_t> file_data;
                        archive_array.get_file_data(filename, file_data);

                        std::ofstream output_file{full_path, std::ios::trunc};
                        output_file.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());

                        std::cout << "File was exported to: " << full_path << "\n";
                    }
                }
            }

            if (select_info.selected_files.empty()) {
                ImGui::SameLine();
                ImGui::Text("No files selected");
            }

            ImGui::End();

            ImGui::Begin("Trees");
            {
                if (filter.Draw()) {
                    root_tree.update_filter(filter);

                    file_names.clear();

                    for (auto&[_, path] : archive_array.hash_to_name) {
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
                            select_info.selected_file = hash_string(sanitize_name(file_names[file_id]), Decima::seed);
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

                        root_tree.draw(select_info.selected_files, select_info.selected_file, archive_array);

                        ImGui::Columns(1);

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::End();

            ImGui::Begin("File info");
            {

                if (select_info.selected_file != 0) {
                    std::string full_path = archive_array.hash_to_name[select_info.selected_file];
                    auto filename = sanitize_name(full_path);
                    auto* file_entry = archive_array.get_file_entry(filename);
                    std::vector<std::string> parts;
                    split(full_path, parts, '/');
                    ImGui::Text("%s", parts[0].c_str());
                    ImGui::LabelText("Size", "%u bytes", file_entry->size);
                    ImGui::LabelText("Hash", "%llu", file_entry->hash);
                    ImGui::LabelText("Entry ID", "%u", file_entry->entry_num);
                    ImGui::LabelText("Offset", "%llu", file_entry->offset);
                }
            }
            ImGui::End();

            ImGui::Begin("File preview");
            {
                if (select_info.selected_file > 0) {
                    const auto filename = sanitize_name(archive_array.hash_to_name.at(select_info.selected_file));

                    ImGui::Text("%s", filename.c_str());

                    if (select_info.preview_file != select_info.selected_file) {
                        select_info.preview_file = select_info.selected_file;
                        archive_array.get_file_data(filename, select_info.file_data);
                    }

                    file_viewer.DrawContents(select_info.file_data.data(), select_info.file_data.size());
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
    auto app = std::make_shared<omc::application>("Application", 1280, 720, true);

    app->push_layer(std::make_shared<MainLayer>(app.get()));

    app->run();
}

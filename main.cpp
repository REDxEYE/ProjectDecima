#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_PERLIN_IMPLEMENTATION

#include <stb_perlin.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>

#include <utility>

#include <iostream>

#include "archive/decima_archive.h"
#include "archive_array.h"
#include "file_tree.h"
#include "imgui_memory_editor.h"

#include <application/application.hpp>
#include <application/opengl_backend.h>
#include <utils.h>

#include "portable-file-dialogs.h"

class MainLayer : public omc::layer {
    Decima::ArchiveArray archive_array;
    std::vector<const char*> file_names;
    FileTree root_tree;
    uint32_t selected_file_hash = 0;
    uint32_t current_open_file_hash = 0;
    std::vector<uint8_t> current_open_file_data;

    int32_t file_id = 0;
    ImGuiTextFilter filter;
    MemoryEditor file_viewer;

public:
    explicit MainLayer(omc::application* app)
        : layer(app) {};

    void on_attach() override
    {
        layer::on_attach();
    }

    void on_detach() override
    {
        layer::on_detach();
    }

    void on_update(double ts) override
    {
        omc::opengl_backend::imgui_new_frame();
        {
            //            ImGui::Begin("DEBUG");
            //            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
            //                        ImGui::GetIO().Framerate);
            //
            //            ImGui::End();

            ImGui::Begin("Test");
            if (ImGui::Button("Folder")) {
                auto folder = pfd::select_folder("Select Death Stranding data folder!").result();

                if (!folder.empty()) {
                    archive_array.open(folder);
                    archive_array.read_content_table();
                    archive_array.read_prefetch_file();
                    file_names.clear();
                    file_names.reserve(archive_array.hash_to_name.size());

                    for (auto& [hash, path] : archive_array.hash_to_name) {
                        file_names.push_back(path.c_str());
                        auto* current_root = &root_tree;

                        std::vector<std::string> split_path;
                        split(path, split_path, '/');

                        for (auto it = split_path.cbegin(); it != split_path.end() - 1; it++)
                            current_root = current_root->add_folder(*it);

//                        if (archive_array.hash_to_archive.find(hash) != archive_array.hash_to_archive.end())
                            current_root->add_file(split_path.back(), hash);
                    }
                }
            }

            if (selected_file_hash != 0) {
                ImGui::Button(archive_array.hash_to_name[selected_file_hash].c_str());
            }

            ImGui::End();

            ImGui::Begin("Trees");
            {
                if (filter.Draw()) {
                    root_tree.update_filter(filter);

                    file_names.clear();
                    for (auto& [_, path] : archive_array.hash_to_name)
                        if (filter.PassFilter(path.c_str()))
                            file_names.push_back(path.c_str());
                }

                ImGui::BeginTabBar("Tabs");
                if (ImGui::BeginTabItem("ListView")) {
                    ImGui::PushItemWidth(-1);
                    if (ImGui::ListBox("TREE", &file_id, file_names.data(), file_names.size(), 50)) {
                        selected_file_hash = hash_string(std::string(file_names[file_id]) + ".core", Decima::seed);
                    }
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

                    root_tree.draw(selected_file_hash, archive_array);

                    ImGui::Columns(1);

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();

            ImGui::Begin("File preview");
            {
                if (selected_file_hash != 0) {
                    if (selected_file_hash != current_open_file_hash) {
                        auto filename = archive_array.hash_to_name.at(selected_file_hash) + ".core";
                        archive_array.get_file_data(filename, current_open_file_data);
                        current_open_file_hash = selected_file_hash;
                    }
                    file_viewer.DrawContents(current_open_file_data.data(), current_open_file_data.size());
                }
            }
            ImGui::End();
        }
        omc::opengl_backend::imgui_render_frame();
    }
};

int main()
{
    auto app = std::make_shared<omc::application>("Application", 1280, 720, true);

    app->push_layer(std::make_shared<MainLayer>(app.get()));

    app->run();
}

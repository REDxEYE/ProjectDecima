#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by i.getsman on 06.08.2020.
//

#include <string>
#include <stdexcept>
#include <filesystem>
#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "glad/glad.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "imgui.h"


#include "utils.h"
#include "archive.h"
#include "archive_array.h"
#include "archive_tree.h"
#include "imgui_memory_editor.h"

#include "portable-file-dialogs.h"



static void glfw_error_handler(int error, const char* message) {
    throw std::runtime_error("GLFW error: " + std::to_string(error) + ": " + message);
}


GLFWwindow* init_glfw(std::pair<uint32_t, uint32_t> window_size, const std::string& title) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(window_size.first, window_size.second, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* hwnd, int width, int height) {
        glViewport(0, 0, width, height);
    });
    glfwSetWindowMaximizeCallback(window, [](GLFWwindow* hwnd, int maximized) {
        int width, height;
        glfwGetWindowSize(hwnd, &width, &height);
        glViewport(0, 0, width, height);
    });

    return window;
}

void init_glad() {
    gladLoadGL();
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void init_imgui(GLFWwindow* window, bool multi_viewport) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    if (multi_viewport) {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui_ImplOpenGL3_NewFrame();


}

void imgui_new_frame() {
//    for(auto viewport: ImGui::GetPlatformIO().Viewports) {
//
//        ImGui::GetPlatformIO().Platform_SetWindowAlpha(viewport,0.5);
//    }
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void imgui_render_frame() {
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

inline void clear(float r = 0, float g = 0, float b = 0, float a = 1,
                         uint32_t flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) {
    glClear(flags);
    glClearColor(r, g, b, a);
}

void split(const std::string& str, std::vector<std::string>& cont, char delim) {
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}


Decima::ArchiveArray archive_array;
std::vector<const char*> file_names;
FileTree root_tree;
SelectionInfo selection_info;
int32_t file_id = 0;
ImGuiTextFilter filter;
MemoryEditor file_viewer;

void user_update(GLFWwindow* window){
    imgui_new_frame();
    {
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

                                if (archive_array.hash_to_archive.find(hash) != archive_array.hash_to_archive.end())
                                    current_root->add_file(split_path.back(), hash, { 0 });
                            }
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
        }
        ImGui::End();

        ImGui::Begin("DEBUG");
        //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0x80,0xFF,0x80,0xFF));
        //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0x80,0x80,0xFF,0xFF));
        //            ImGui::PushStyleColor(ImGuiCol_Text,IM_COL32(0xF0,0x80,0xFF,0xFF));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Test");
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

        ImGui::Separator();

        if (ImGui::Button("Add by name"))
            ImGui::OpenPopup("By name");

        if (ImGui::BeginPopup("By name")) {
            static char path[512];
            ImGui::InputText("File name", path, IM_ARRAYSIZE(path));
            if (ImGui::Button("Add to selection!")) {
                std::string str_path(path);
                uint64_t file_hash = hash_string(sanitize_name(str_path), Decima::seed);
                if (archive_array.get_file_entry(file_hash).has_value()) {
                    archive_array.hash_to_name[file_hash] = str_path;
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

        if (ImGui::Button("Export selected file(-s)") && !selection_info.selected_files.empty()) {
            const auto base_folder = pfd::select_folder("Choose destination folder").result();

            if (!base_folder.empty()) {
                for (const auto selected_file : selection_info.selected_files) {
                    const auto filename = sanitize_name(archive_array.hash_to_name.at(selected_file));

                    std::filesystem::path full_path = std::filesystem::path(base_folder) / filename;
                    std::filesystem::create_directories(full_path.parent_path());

                    auto file = archive_array.query_file(filename);
                    file.unpack(0);
                    std::ofstream output_file { full_path, std::ios::binary };
                    output_file.write(reinterpret_cast<const char*>(file.storage.data()), file.storage.size());

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
                        selection_info.file = archive_array.query_file(selection_info.selected_file);
                        selection_info.file.unpack(0);
                    }

                    std::uint64_t file_type;
                    std::memcpy(reinterpret_cast<std::uint8_t*>(&file_type), selection_info.file.storage.data(), 8);

                    const auto type_handler = root_tree.file_type_handlers.find(file_type);

                    if (type_handler == root_tree.file_type_handlers.end()) {
                        file_viewer.DrawContents(selection_info.file.storage.data(), selection_info.file.storage.size());
                    } else {
                        imemstream stream { selection_info.file.storage };
                        type_handler->second.render_fn(stream);
                    }

                } else {
                    ImGui::Text("Error getting file info!");
                }
            } else {
                ImGui::Text("No file selected");
            }
        }
        ImGui::End();
    }
    imgui_render_frame();
}

void user_init(){
    file_viewer.WriteFn = [](auto, auto, auto) {
        /* Dummy write function because
         * ReadOnly forbids any selection. */
    };
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    {
        FileTypeHandler handler;
        handler.name = "Localization";
        handler.render_fn = [](imemstream& stream) {
            ImGui::Text("Custom handler for localization goes here");
            ImGui::TextDisabled("Someday...");
        };

        root_tree.file_type_handlers.insert(std::make_pair(0x31be502435317445, std::move(handler)));
    }
}

int main() {
    auto* window = init_glfw({1280, 720}, "Project Decima");
    init_glad();
    init_imgui(window, false);
    user_init();
    while (!glfwWindowShouldClose(window)) {
        clear();

        user_update(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
#pragma clang diagnostic pop
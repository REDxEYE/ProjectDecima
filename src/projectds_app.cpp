//
// Created by i.getsman on 06.08.2020.
//

#include "projectds_app.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ProjectDS::ProjectDS(Decima::ArchiveManager&& manager, const std::pair<uint32_t, uint32_t>& windowSize, const std::string& title)
    : App(windowSize, title)
    , archive_manager(std::move(manager)) {}

void ProjectDS::begin_frame_user() {
    App::begin_frame_user();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void ProjectDS::end_frame_user() {
    App::end_frame_user();
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

void ProjectDS::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    if (m_multi_viewport) {
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui_ImplOpenGL3_NewFrame();
}









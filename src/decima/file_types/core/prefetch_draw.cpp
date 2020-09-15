#include "decima/file_types/core/prefetch.hpp"

#include <imgui.h>

void Decima::Prefetch::draw() {
    ImGui::Columns(2);

    ImGui::Text("Path");
    ImGui::NextColumn();

    ImGui::Text("Size");
    ImGui::NextColumn();

    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 200);
    ImGui::SetColumnWidth(1, 200);

    ImGui::Separator();

    ImGuiListClipper clipper(paths.size());

    while (clipper.Step()) {
        for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
            ImGui::TextUnformatted(paths.at(index).data().data());
            ImGui::NextColumn();

            ImGui::Text("%d bytes", sizes.at(index));
            ImGui::NextColumn();

            ImGui::Separator();
        }
    }

    ImGui::Columns(1);
}

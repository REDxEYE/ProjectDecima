#include "decima/serializable/object/prefetch.hpp"

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

    ImGuiListClipper clipper(paths.data().size());

    while (clipper.Step()) {
        for (auto index = clipper.DisplayStart; index < clipper.DisplayEnd; index++) {
            ImGui::TextUnformatted(paths.data().at(index).data().data());
            ImGui::NextColumn();

            ImGui::Text("%d bytes", sizes.data().at(index));
            ImGui::NextColumn();

            ImGui::Separator();
        }
    }

    ImGui::Columns(1);
}

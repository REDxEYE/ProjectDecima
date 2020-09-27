#include "decima/serializable/object/resource/index_array_resource.hpp"

#include <imgui.h>

void Decima::IndexArrayResource::draw() {
    ImGui::Columns(2);

    {
        ImGui::Text("Property");
        ImGui::NextColumn();

        ImGui::Text("Value");
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::SetColumnWidth(0, 200);
    ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 200);

    {
        ImGui::Text("Indices count");
        ImGui::NextColumn();

        ImGui::Text("%u", indices_count);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Flags");
        ImGui::NextColumn();

        ImGui::Text("%u", flags);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Index type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Is streaming");
        ImGui::NextColumn();

        ImGui::Text("%u", is_streaming);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Resource UUID");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(resource_uuid).c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}
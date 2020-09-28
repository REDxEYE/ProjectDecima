#include "decima/serializable/object/resource/primitive_resource.hpp"

#include <imgui.h>

void Decima::PrimitiveResource::draw() {
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
        ImGui::Text("Flags");
        ImGui::NextColumn();

        ImGui::Text("%u", flags);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Bounding Box");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(bounding_box).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Start index");
        ImGui::NextColumn();

        ImGui::Text("%u", start_index);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("End index");
        ImGui::NextColumn();

        ImGui::Text("%u", end_index);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Hash");
        ImGui::NextColumn();

        ImGui::Text("%#08x", hash);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Vertex array");
        ImGui::NextColumn();

        vertex_array.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Index array");
        ImGui::NextColumn();

        index_array.draw();
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("SKD tree");
        ImGui::NextColumn();

        skd_tree.draw();
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

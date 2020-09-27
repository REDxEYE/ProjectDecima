#include "decima/serializable/object/resource/vertex_array_resource.hpp"

#include <imgui.h>

void Decima::VertexStreamData::draw() {
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
        ImGui::Text("Storage type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(storage_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Element type");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(element_type).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Slots used");
        ImGui::NextColumn();

        ImGui::Text("%u", slots_used);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Offset");
        ImGui::NextColumn();

        ImGui::Text("%u", offset);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::Columns(1);
}

void Decima::VertexStreamInfo::draw() {
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
        ImGui::Text("Stride");
        ImGui::NextColumn();

        ImGui::Text("%u", stride);
        ImGui::NextColumn();

        ImGui::Separator();
    }

    {
        ImGui::Text("Resource UUID");
        ImGui::NextColumn();

        ImGui::Text("%s", Decima::to_string(resource_uuid).c_str());
        ImGui::NextColumn();

        ImGui::Separator();
    }

    ImGui::Columns(1);

    if (ImGui::TreeNodeEx("Descriptors", ImGuiTreeNodeFlags_SpanFullWidth)) {
        for (std::size_t entry_index = 0; entry_index < descriptors.data().size(); entry_index++) {
            const auto entry_name = "Descriptor #" + std::to_string(entry_index);

            if (ImGui::TreeNodeEx(entry_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                descriptors.data().at(entry_index).draw();
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}

void Decima::VertexArrayResource::draw() {
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
        ImGui::Text("Vertex count");
        ImGui::NextColumn();

        ImGui::Text("%u", vertex_count);
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

    ImGui::Columns(1);

    if (ImGui::TreeNodeEx("Streams", ImGuiTreeNodeFlags_SpanFullWidth)) {
        for (std::size_t entry_index = 0; entry_index < vertex_stream_info.size(); entry_index++) {
            const auto entry_name = "Stream #" + std::to_string(entry_index);

            if (ImGui::TreeNodeEx(entry_name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                vertex_stream_info.at(entry_index).draw();
                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}
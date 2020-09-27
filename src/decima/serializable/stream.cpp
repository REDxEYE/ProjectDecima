#include "decima/serializable/stream.hpp"

#include <imgui.h>

void Decima::Stream::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    m_name.parse(buffer, file);
    buffer = buffer.skip(20);
    m_offs = buffer.get<decltype(m_offs)>();
    m_size = buffer.get<decltype(m_size)>();

    auto& stream_file = manager.query_file(m_name.data() + ".core.stream").value().get();
    m_data = stream_file.contents;
}

void Decima::Stream::draw() {
    ImGui::Columns(2);
    {
        ImGui::SetColumnWidth(-1, 80);
        ImGui::Text("Field");
        ImGui::NextColumn();
        ImGui::Text("Value");
        ImGui::NextColumn();

        ImGui::Separator();

        ImGui::Text("Name");
        ImGui::NextColumn();
        m_name.draw([](const auto& name) { return name + ".core.stream"; });
        ImGui::NextColumn();

        ImGui::Separator();

        ImGui::Text("Offset");
        ImGui::NextColumn();
        ImGui::Text("%u", m_offs);
        ImGui::NextColumn();

        ImGui::Separator();

        ImGui::Text("Length");
        ImGui::NextColumn();
        ImGui::Text("%u", m_size);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}

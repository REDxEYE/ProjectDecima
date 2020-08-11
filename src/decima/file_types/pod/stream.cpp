#include "decima/file_types/pod/stream.hpp"
#include <imgui.h>

void Decima::Stream::parse(Source& stream) {
    m_name.parse(stream);
    stream.seek(ash::seek_dir::cur, 20);
    m_offs = stream.read<std::uint32_t>();
    m_size = stream.read<std::uint32_t>();
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
        m_name.draw();
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

#include "decima/file_types/pod/stream.hpp"
#include <imgui.h>

void Decima::Stream::parse(ArchiveArray& archives, Source& stream) {
    m_name.parse(stream);
    stream.seek(ash::seek_dir::cur, 20);
    m_offs = stream.read<std::uint32_t>();
    m_size = stream.read<std::uint32_t>();

    auto& stream_file = archives.query_file(m_name.data() + ".core.stream").value().get();
    stream_file.unpack();

    m_data = stream_file.storage;
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

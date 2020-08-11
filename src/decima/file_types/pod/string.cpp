#include "decima/file_types/pod/string.hpp"
#include <imgui.h>

void Decima::String::parse(Source& stream) {
    const auto size = stream.read<std::uint32_t>();
    if (size > 0) {
        m_data.resize(size);
        stream.read_exact(m_data);
    }
}

void Decima::String::draw() {
    ImGui::TextWrapped("%s", m_data.c_str());
}

void Decima::StringHashed::parse(Source& stream) {
    const auto size = stream.read<std::uint32_t>();
    m_hash = stream.read<std::uint32_t>();
    if (size > 0) {
        m_data.resize(size);
        stream.read_exact(m_data);
    }
}

void Decima::StringHashed::draw() {
    ImGui::TextWrapped("%s", m_data.c_str());
}
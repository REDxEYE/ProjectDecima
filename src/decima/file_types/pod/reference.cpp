#include "decima/file_types/pod/reference.hpp"
#include <imgui.h>

void Decima::Reference::parse(ArchiveArray& archives, Source& stream) {
    m_mode = stream.read<decltype(m_mode)>();
    if (m_mode != ReferenceLoadMode::NotPresent)
        m_guid.parse(stream);
    if (m_mode >= ReferenceLoadMode::ImmediateCoreFile)
        m_file.parse(stream);
}

void Decima::Reference::draw() {
    m_guid.draw();
    ImGui::SameLine();

    std::stringstream buffer;
    buffer << m_mode;
    ImGui::TextDisabled("Reference (%s)", buffer.str().c_str());

    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        m_file.draw();
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

namespace Decima {
    std::ostream& operator<<(std::ostream& os, ReferenceLoadMode mode) {
        switch (mode) {
        case ReferenceLoadMode::NotPresent:
            return os << "Not present";
        case ReferenceLoadMode::Embedded:
            return os << "Embedded";
        case ReferenceLoadMode::ImmediateCoreFile:
            return os << "Immediate CoreFile";
        case ReferenceLoadMode::CoreFile:
            return os << "CoreFile";
        case ReferenceLoadMode::WorkOnly:
            return os << "Work only";
        default:
            return os << "Unknown";
        }
    }
}

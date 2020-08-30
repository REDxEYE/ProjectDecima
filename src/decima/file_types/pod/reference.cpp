#include "decima/file_types/pod/reference.hpp"
#include "decima/core_file.hpp"
#include <decima/archive/archive_array.hpp>
#include <imgui.h>

std::vector<Decima::Reference*> Decima::global_refs {};
std::vector<std::pair<Decima::Reference*, Decima::CoreFile*>> Decima::local_refs {};

void Decima::Reference::parse(ash::buffer& buffer) {
    m_mode = buffer.get<decltype(m_mode)>();
    if (m_mode != ReferenceLoadMode::NotPresent)
        m_guid.parse(buffer);
    if (m_mode >= ReferenceLoadMode::ImmediateCoreFile)
        m_file.parse(buffer);
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

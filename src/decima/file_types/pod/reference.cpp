#include "decima/file_types/pod/reference.hpp"
#include "decima/core_file.h"
#include <decima/archive/archive_array.h>
#include <imgui.h>

std::vector<Decima::Reference*> Decima::global_refs{};
std::vector<std::pair<Decima::Reference*, Decima::CoreFile*>> Decima::local_refs{};

void Decima::Reference::parse(Source& stream) {
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

void Decima::Reference::resolve_global_refs(ArchiveArray& arch_array) {
    for (auto* ref:global_refs) {
        auto core_file = arch_array.query_file(ref->file().data() + ".core");
        if (core_file.is_valid()) {
            for (auto& entry:core_file.entries) {
                if (entry->guid.hash() == ref->guid().hash())
                    ref->ref = std::weak_ptr(entry);
            }
        }
    }
}

void Decima::Reference::resolve_local_refs() {
    for (auto&[ref, core_file]:local_refs) {
        for (auto& entry:core_file->entries) {
            if (entry->guid.hash() == ref->guid().hash())
                ref->ref = std::weak_ptr(entry);
        }
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

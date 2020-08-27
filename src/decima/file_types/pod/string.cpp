#include "decima/file_types/pod/string.hpp"
#include <imgui.h>

static void string_draw(const std::string& data, Decima::StringMutator mutator) {
    ImGui::TextWrapped("%s", data.c_str());

    if (ImGui::BeginPopupContextItem(data.c_str())) {
        if (ImGui::Selectable("Copy to clipboard"))
            ImGui::SetClipboardText(mutator(data).c_str());
        ImGui::EndPopup();
    }
}

static void string_draw(const std::string& data) {
    static const Decima::StringMutator default_mutator = [](const auto& str) { return str; };
    string_draw(data, default_mutator);
}

void Decima::String::parse(Source& stream) {
    const auto size = stream.read<std::uint32_t>();
    if (size > 0) {
        m_data.resize(size);
        stream.read_exact(m_data);
    }
}

void Decima::String::draw() {
    string_draw(m_data);
}

void Decima::String::draw(Decima::StringMutator mutator) {
    string_draw(m_data, mutator);
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
    string_draw(m_data);
}

void Decima::StringHashed::draw(Decima::StringMutator mutator) {
    string_draw(m_data, mutator);
}

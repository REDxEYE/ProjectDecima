#include "decima/serializable/string.hpp"

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

void Decima::String::parse(ash::buffer& buffer, CoreFile& file) {
    const auto size = buffer.get<std::uint32_t>();

    if (size > 0) {
        m_data.resize(size);
        buffer.get(m_data);
    }
}

void Decima::String::draw() {
    string_draw(m_data);
}

void Decima::String::draw(Decima::StringMutator mutator) {
    string_draw(m_data, mutator);
}

void Decima::StringHashed::parse(ash::buffer& buffer, CoreFile& file) {
    const auto size = buffer.get<std::uint32_t>();

    if (size > 0) {
        m_hash = buffer.get<decltype(m_hash)>();
        m_data.resize(size);
        buffer.get(m_data);
    }
}

void Decima::StringHashed::draw() {
    string_draw(m_data);
}

void Decima::StringHashed::draw(Decima::StringMutator mutator) {
    string_draw(m_data, mutator);
}

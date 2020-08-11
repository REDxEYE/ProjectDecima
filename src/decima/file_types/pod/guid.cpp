#include "decima/file_types/pod/guid.hpp"
#include <imgui.h>

void Decima::GUID::parse(Source& stream) {
    m_data_8[0] = stream.read<std::uint64_t>();
    m_data_8[1] = stream.read<std::uint64_t>();
}

void Decima::GUID::draw() {
    ImGui::Text("%08x-%04x-%04x-%04x-%012llx",
        std::uint32_t(m_data_8[0] >> 32),
        std::uint16_t(m_data_8[0] >> 16 & 0xffff),
        std::uint16_t(m_data_8[0] >> 0 & 0xffff),
        std::uint16_t(m_data_8[1] >> 48),
        std::uint64_t(m_data_8[1] >> 0));
}

#include <ostream>
#include <iomanip>

namespace Decima {
    std::ostream& operator<<(std::ostream& os, GUID guid) {
        return os << std::hex << std::setfill('0')
                  << std::setw(8) << (guid.m_data_8[0] >> 32) << '-'
                  << std::setw(4) << (guid.m_data_8[0] >> 16 & 0xffff) << '-'
                  << std::setw(4) << (guid.m_data_8[0] >> 0 & 0xffff) << '-'
                  << std::setw(4) << (guid.m_data_8[1] >> 48) << '-'
                  << std::setw(12) << (guid.m_data_8[1] >> 0 & 0xffffffffffff);
    }
}
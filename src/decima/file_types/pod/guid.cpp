#include "decima/file_types/pod/guid.hpp"
#include <imgui.h>

void Decima::GUID::parse(ash::buffer& buffer) {
    buffer.get(m_data_1.data(), sizeof(m_data_1));
}

void Decima::GUID::draw() {
    ImGui::Text("%08x-%04x-%04x-%04x-%012llx",
        std::uint32_t(m_data_8[0] >> 32 & 0xffffffff),
        std::uint16_t(m_data_8[0] >> 16 & 0xffff),
        std::uint16_t(m_data_8[0] >> 0 & 0xffff),
        std::uint16_t(m_data_8[1] >> 48 & 0xffff),
        std::uint64_t(m_data_8[1] >> 0 & 0xffffffffffff));
}

#include <ostream>
#include <iomanip>

namespace Decima {
    std::ostream& operator<<(std::ostream& os, GUID guid) {
        const auto flags = os.flags();
        os << std::hex << std::setfill('0')
           << std::setw(8) << (guid.m_data_8[0] >> 32) << '-'
           << std::setw(4) << (guid.m_data_8[0] >> 16 & 0xffff) << '-'
           << std::setw(4) << (guid.m_data_8[0] >> 0 & 0xffff) << '-'
           << std::setw(4) << (guid.m_data_8[1] >> 48) << '-'
           << std::setw(12) << (guid.m_data_8[1] >> 0 & 0xffffffffffff);
        os.flags(flags);
        return os;
    }
}
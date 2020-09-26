#pragma once

#include <array>
#include <functional>

#include "decima/shared.hpp"
#include "util/buffer.hpp"

namespace Decima {
    class GUID {
    public:
        void parse(ash::buffer& buffer);
        void draw();

        inline std::array<std::uint8_t, 16> data() const noexcept { return m_data_1; }

        inline std::size_t hash() const noexcept {
            constexpr std::hash<std::uint64_t> hash {};
            return hash(m_data_8[0]) ^ hash(m_data_8[1]);
        }

    private:
        friend std::string Decima::to_string(const Decima::GUID& value);

        union {
            std::array<std::uint8_t, 16> m_data_1;
            std::array<std::uint64_t, 2> m_data_8;
        };
    };

    static_assert(sizeof(GUID) == 16);
}

template <>
inline std::string Decima::to_string(const Decima::GUID& value) {
    std::string buffer(36, ' ');
    std::sprintf(buffer.data(), "%08x-%04x-%04x-%04x-%012llx",
        std::uint32_t(value.m_data_8[0] >> 32 & 0xffffffff),
        std::uint16_t(value.m_data_8[0] >> 16 & 0xffff),
        std::uint16_t(value.m_data_8[0] >> 0 & 0xffff),
        std::uint16_t(value.m_data_8[1] >> 48 & 0xffff),
        std::uint64_t(value.m_data_8[1] >> 0 & 0xffffffffffff));
    return buffer;
}
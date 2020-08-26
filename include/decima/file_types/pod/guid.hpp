#pragma once

#include <array>
#include <functional>

#include "decima/shared.hpp"

namespace Decima {
    class GUID {
    public:
        void parse(Source& stream);
        void draw();

        inline std::array<std::uint8_t, 16> data() const noexcept { return m_data_1; }

        inline std::size_t hash() const noexcept {
            constexpr std::hash<std::uint64_t> hash {};
            return hash(m_data_8[0]) ^ hash(m_data_8[1]);
        }

    private:
        friend std::ostream& operator<<(std::ostream&, Decima::GUID);

        union {
            std::array<std::uint8_t, 16> m_data_1;
            std::array<std::uint64_t, 2> m_data_8;
        };
    };

    static_assert(sizeof(GUID) == 16);
}
#pragma once

#include "decima/file_types/pod/pod.hpp"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Stream : public POD {
    public:
        void parse(Source& stream);
        void draw();

        inline const Decima::String& name() const noexcept { return m_name; }
        inline std::uint32_t offset() const noexcept { return m_offs; }
        inline std::uint32_t size() const noexcept { return m_size; }

    private:
        Decima::String m_name;
        std::uint32_t m_offs;
        std::uint32_t m_size;
    };
}
#pragma once

#include "decima/archive/archive_array.hpp"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Stream {
    public:
        void parse(ArchiveArray& archives, ash::buffer& buffer);
        void draw();

        inline const Decima::String& name() const noexcept { return m_name; }
        inline std::uint32_t offset() const noexcept { return m_offs; }
        inline std::uint32_t size() const noexcept { return m_size; }
        inline const std::vector<char>& data() const noexcept { return m_data; }

    private:
        Decima::String m_name;
        std::uint32_t m_offs;
        std::uint32_t m_size;
        std::vector<char> m_data;
    };
}

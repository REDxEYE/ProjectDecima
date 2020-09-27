#pragma once

#include "decima/archive/archive_manager.hpp"
#include "decima/serializable/string.hpp"

namespace Decima {
    class Stream {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file);
        void draw();

        inline const String& name() const noexcept { return m_name; }
        inline std::uint32_t offset() const noexcept { return m_offs; }
        inline std::uint32_t size() const noexcept { return m_size; }
        inline const std::vector<char>& data() const noexcept { return m_data; }

    private:
        String m_name;
        std::uint32_t m_offs;
        std::uint32_t m_size;
        std::vector<char> m_data;
    };
}

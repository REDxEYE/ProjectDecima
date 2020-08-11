#pragma once

#include "decima/archive/archive_array.h"
#include "decima/file_types/pod/pod.hpp"
#include "decima/file_types/pod/string.hpp"

namespace Decima {
    class Stream : public POD {
    public:
        void parse(ArchiveArray& archives, Source& stream);
        void draw();

        inline const Decima::String& name() const noexcept { return m_name; }
        inline std::uint32_t offset() const noexcept { return m_offs; }
        inline std::uint32_t size() const noexcept { return m_size; }
        inline const std::vector<std::uint8_t>& data() const noexcept { return m_data; }

    private:
        Decima::String m_name;
        std::uint32_t m_offs;
        std::uint32_t m_size;
        std::vector<std::uint8_t> m_data;
    };
}
#pragma once

#include "decima/archive/archive_array.h"
#include "decima/file_types/pod/pod.hpp"

namespace Decima {
    enum class ReferenceLoadMode : std::uint8_t {
        NotPresent = 0x00,
        Embedded = 0x01,
        ImmediateCoreFile = 0x02,
        CoreFile = 0x03,
        WorkOnly = 0x05
    };

    std::ostream& operator<<(std::ostream&, ReferenceLoadMode);

    class Reference : public POD {
    public:
        void parse(ArchiveArray& archives, Source& stream);
        void draw();

        inline ReferenceLoadMode mode() const noexcept { return m_mode; }
        inline GUID guid() const noexcept { return m_guid; }
        inline StringHashed file() const noexcept { return m_file; }

    private:
        ReferenceLoadMode m_mode;
        GUID m_guid;
        StringHashed m_file;
    };
}
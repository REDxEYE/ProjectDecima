#pragma once

#include "decima/serializable/guid.hpp"
#include "decima/serializable/serializable.hpp"
#include "decima/serializable/string.hpp"

namespace Decima {
    class ArchiveManager;
    class CoreFile;
    class CoreObject;

    enum class RefLoadMode : std::uint8_t {
        NotPresent = 0,
        Embedded = 1,
        ImmediateCoreFile = 2,
        CoreFile = 3,
        WorkOnly = 5
    };

    class Ref : public CoreSerializable {
    public:
        void parse(ash::buffer& buffer);
        void draw();

        inline RefLoadMode mode() const { return m_mode; }
        inline GUID guid() const { return m_guid; }
        inline StringHashed file() const { return m_file; }

    private:
        RefLoadMode m_mode;
        GUID m_guid;
        StringHashed m_file;
    };
}

template <>
inline std::string Decima::to_string(const Decima::RefLoadMode& value) {
    switch (value) {
    case RefLoadMode::NotPresent:
        return "Not present";
    case RefLoadMode::Embedded:
        return "Embedded";
    case RefLoadMode::ImmediateCoreFile:
    case RefLoadMode::CoreFile:
        return "CoreFile";
    case RefLoadMode::WorkOnly:
        return "Work only";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}
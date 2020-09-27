#pragma once

#include "decima/serializable/object/resource/resource.hpp"
#include "decima/serializable/array.hpp"
#include "decima/shared.hpp"

namespace Decima {
    enum class IndexArrayType : std::uint32_t {
        Index16 = 0,
        Index32 = 1
    };

    class IndexArrayResource : public Resource {
    public:
        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) override;
        void draw() override;

    public:
        std::uint32_t indices_count;
        std::uint32_t flags;
        IndexArrayType type;
        std::uint32_t is_streaming;
        GUID resource_uuid;
    };
}

template <>
inline std::string Decima::to_string(const IndexArrayType& value) {
    switch (value) {
    case IndexArrayType::Index16:
        return "Index16";
    case IndexArrayType::Index32:
        return "Index32";
    default:
        return "Unknown: " + std::to_string(static_cast<std::size_t>(value));
    }
}
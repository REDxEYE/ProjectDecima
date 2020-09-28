#pragma once

#include "decima/serializable/object/common/vec.hpp"

namespace Decima {
    class BBox2 {
    public:
        Vec2 min;
        Vec2 max;
    };

    class BBox3 {
    public:
        Vec3 min;
        Vec3 max;
    };
}

template <>
inline std::string Decima::to_string(const BBox2& value) {
    return Decima::to_string(value.min) + " - " + Decima::to_string(value.max);
}

template <>
inline std::string Decima::to_string(const BBox3& value) {
    return Decima::to_string(value.min) + " - " + Decima::to_string(value.max);
}

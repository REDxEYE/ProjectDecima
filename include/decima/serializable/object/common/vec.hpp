#pragma once

#include <sstream>

#include "decima/shared.hpp"

namespace Decima {
    class Vec2 {
    public:
        float x;
        float y;
    };

    class Vec3 {
    public:
        float x;
        float y;
        float z;
    };
}

template <>
inline std::string Decima::to_string(const Vec2& value) {
    std::stringstream buffer;
    buffer << '(' << std::scientific << value.x << ", " << value.y << ')';
    return buffer.str();
}

template <>
inline std::string Decima::to_string(const Vec3& value) {
    std::stringstream buffer;
    buffer << '(' << std::scientific << value.x << ", " << value.y  << ", " << value.z << ')';
    return buffer.str();
}

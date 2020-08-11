#pragma once

#include "ash.hpp"

namespace Decima {
    struct POD {
        using Source = ash::buffered_source<std::vector<uint8_t>>;
    };
}
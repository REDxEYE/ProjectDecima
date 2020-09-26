#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "utils.hpp"

namespace Decima {
    static constexpr uint8_t seed = 0x2A;
    static constexpr uint32_t encryption_key_1[4] = { 0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E };
    static constexpr uint32_t encryption_key_2[4] = { 0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F };
}

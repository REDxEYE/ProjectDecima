//
// Created by i.getsman on 03.08.2020.
//

#ifndef PROJECTDS_CONSTANT_HPP
#define PROJECTDS_CONSTANT_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Decima {

    enum class Version : uint32_t {
        default_version = 0x20304050,
        encrypted_version = 0x21304050
    };

    static const uint8_t seed = 0x2A;
    static const uint32_t encryption_key_1[4] = { 0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E };
    static const uint32_t encryption_key_2[4] = { 0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F };

    static const std::unordered_map<uint64_t, std::string> known_file_types = {
        { 0xA664164D69FD2B38, "Texture" },
        { 11754932426750725422u, "TextureSet" },
        { 1638019067902823838, "Model" },
        { 1288541470903713382, "Armature" },
    };

}

#endif //PROJECTDS_CONSTANT_HPP

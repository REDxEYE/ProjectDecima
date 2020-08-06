//
// Created by i.getsman on 03.08.2020.
//

#ifndef PROJECTDS_CONSTANTS_HPP
#define PROJECTDS_CONSTANTS_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Decima {

    class FileMagics {
    };

    class DeathStranding_FileMagics : public FileMagics {
    public:
        static constexpr uint64_t Texture = 0xa664164d69fd2b38;
        static constexpr uint64_t TextureSet = 0xa321e8c307328d2e;
        static constexpr uint64_t Model = 0x16bb69a9e5aa0d9e;
        static constexpr uint64_t Armature = 0x11e1d1a40b933e66;
        static constexpr uint64_t Localization = 0x31be502435317445;
    };

    enum class Version : uint32_t {
        default_version = 0x20304050,
        encrypted_version = 0x21304050
    };

    static constexpr uint8_t seed = 0x2A;
    static constexpr uint32_t encryption_key_1[4] = {0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E};
    static constexpr uint32_t encryption_key_2[4] = {0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F};

    static const std::unordered_map<uint64_t, std::string> known_file_types = {
            {0xa664164d69fd2b38, "Texture"},
            {0xa321e8c307328d2e, "TextureSet"},
            {0x16bb69a9e5aa0d9e, "Model"},
            {0x11e1d1a40b933e66, "Armature"},
            {0x31be502435317445, "Localization"}
    };

}

#endif //PROJECTDS_CONSTANTS_HPP

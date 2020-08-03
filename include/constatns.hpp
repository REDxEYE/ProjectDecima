//
// Created by i.getsman on 03.08.2020.
//

#ifndef PROJECTDS_CONSTATNS_HPP
#define PROJECTDS_CONSTATNS_HPP

#include "cstdint"

namespace Decima{

    enum class Version:uint32_t {
        default_version = 0x20304050,
        encrypted_version = 0x21304050
    };


    static const uint8_t seed = 0x2A;
    static const uint32_t murmur_salt[4] = { 0x0FA3A9443, 0x0F41CAB62, 0x0F376811C, 0x0D2A89E3E };
    static const uint32_t murmur_salt2[4] = { 0x06C084A37, 0x07E159D95, 0x03D5AF7E8, 0x018AA7D3F };

}

#endif //PROJECTDS_CONSTATNS_HPP

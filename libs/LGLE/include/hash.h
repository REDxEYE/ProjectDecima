//
// Created by MED45 on 13.01.2020.
//

#ifndef OPENMC_HASH_H
#define OPENMC_HASH_H
#include <cstdlib>
#include <cstdint>

namespace omc {
        static inline uint32_t murmur_32_scramble(uint32_t k);
        uint32_t murmur3_32(const uint8_t *key, size_t len, uint32_t seed);
}


#endif //OPENMC_HASH_H

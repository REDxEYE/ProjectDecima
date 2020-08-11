//
// Created by i.getsman on 06.08.2020.
//

#include <fstream>
#include <iostream>
#include "decima/file_types/core/core.h"
#include "decima/file_types/core/translation.hpp"
#include "decima/file_types/core/texture.h"
#include "decima/file_types/core/texture_set.h"
#include "decima/constants.hpp"
#include "utils.h"

int main() {
    std::ifstream file("F:\\SteamLibrary\\steamapps\\common\\Death Stranding\\dump\\ds\\models\\characters\\sam_sam\\core\\sam_textures\\textures\\sam_body_naked_v00_set.core", std::ios::binary);
    file.seekg(0, std::ios::end);
    uint64_t stream_size = file.tellg();
    file.seekg(0);

    while (file.tellg() < stream_size) {
        uint64_t magic;
        file.read(reinterpret_cast<char*>(&magic), 8);
        file.seekg(-8, std::ios::cur);

        }
    }
    return 0;
}
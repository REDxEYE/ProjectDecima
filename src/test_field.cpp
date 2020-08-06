//
// Created by i.getsman on 06.08.2020.
//

#include <fstream>
#include <iostream>
#include "decima/file_types/core.h"
#include "decima/constants.hpp"
#include "utils.h"

int main() {
    std::ifstream file("C:\\Users\\i.getsman\\Downloads\\Telegram Desktop\\simpletext (2).core",std::ios::binary);
    file.seekg(0,std::ios::end);
    uint64_t stream_size = file.tellg();
    file.seekg(0);
    Decima::CoreFile core_file;

    while (file.tellg()<stream_size) {

        std::cout<<"Reading from " <<file.tellg()<<'\n';
        core_file.parse(file);
        std::string file_type_name = uint64_to_hex(core_file.header.filetype);
        if (Decima::known_file_types.find(core_file.header.filetype) != Decima::known_file_types.end()) {
            file_type_name = Decima::known_file_types.at(core_file.header.filetype);
        }
        std::cout << "Header: " << file_type_name << '\n';
        std::cout << "\tSize: " << core_file.header.file_size << '\n';
        file.seekg(core_file.header.file_size-16, std::ios::seekdir::_S_cur);

    }
    return 0;
}
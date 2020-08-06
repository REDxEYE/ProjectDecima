//
// Created by MED45 on 03.08.2020.
//

#include <string>
#include <ostream>
#include <iostream>
#include <iomanip>

#include "utils.h"
#include "decima/constants.hpp"
#include "MurmurHash3.h"
#include "md5.h"

int main() {
    while (1) {
        std::cout << "Input>>";
        std::string string_to_hash;
        std::cin >> string_to_hash;
        if (string_to_hash == "q")
            break;
        std::cout << "CORE:        " << hash_string(sanitize_name(string_to_hash), Decima::seed) << std::setw(10) << " :: " << sanitize_name(string_to_hash) << std::endl;
        std::cout << "NO CORE:     " << hash_string(string_to_hash, Decima::seed) << std::setw(10) << " :: " << string_to_hash << std::endl;
        std::cout << "STREAM:      " << hash_string(string_to_hash + ".stream", Decima::seed) << std::setw(10) << " :: " << string_to_hash + ".stream" << std::endl;
        std::cout << "STREAM CORE: " << hash_string(string_to_hash + "_stream.core", Decima::seed) << std::setw(10) << " :: " << string_to_hash + "_stream.core" << std::endl;
        std::cout << std::endl;
    }
}
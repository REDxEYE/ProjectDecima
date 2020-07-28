//
// Created by i.getsman on 05.03.2020.
//

#include <utils.h>
#include <sstream>
#include <algorithm>

void split(const std::string& str, std::vector<std::string>& cont, char delim) {
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

std::string upper_case(std::string str) {
    std::string tmp(str);
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    return tmp;
}

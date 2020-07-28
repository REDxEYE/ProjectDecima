//
// Created by MED45 on 14.01.2020.
//

#ifndef OPENMC_UTILS_H
#define OPENMC_UTILS_H

#include <functional>
#include "glad/glad.h"
#include <string>
#include <vector>



//bool update_texture(GLuint texture_id, cv::Mat& tex) {
//    glBindTexture(GL_TEXTURE_2D, texture_id);
//    cv::Mat tmp = tex.clone();
//    cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tmp.cols, tmp.rows, GL_RGB, GL_UNSIGNED_BYTE, tmp.ptr());
//    glBindTexture(GL_TEXTURE_2D, 0);
//    return true;
//}

void split(const std::string& str, std::vector<std::string>& cont, char delim);

std::string upper_case(std::string str);

#endif //OPENMC_UTILS_H

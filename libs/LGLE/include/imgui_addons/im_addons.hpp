//
// Created by i.getsman on 23.07.2020.
//

#ifndef LGLE_IM_ADDONS_HPP
#define LGLE_IM_ADDONS_HPP

#include "glm/glm.hpp"
#include "glm/gtx/scalar_multiplication.hpp"

#define IM_VEC2_CLASS_EXTRA                                                 \
        ImVec2(const glm::vec2 & f) { x = f.x; y = f.y; }                       \
        operator  glm::vec2() const { return  glm::vec2(x,y); }

#include <imgui.h>

#endif //LGLE_IM_ADDONS_HPP

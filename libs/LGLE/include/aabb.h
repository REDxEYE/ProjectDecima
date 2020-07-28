//
// Created by MED45 on 23.01.2020.
//

#ifndef OPENMC_AABB_H
#define OPENMC_AABB_H

#include <glm/glm.hpp>

namespace omc {
    class AABB {
    public:
        glm::vec3 min;
        glm::vec3 max;
        AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
    };

}
#endif //OPENMC_AABB_H

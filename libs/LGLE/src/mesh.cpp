//
// Created by MED45 on 17.01.2020.
//

#include <mesh.h>

#include <utility>


omc::mesh::mesh(std::vector<glm::vec3> positions) : positions(std::move(positions)) {
}

omc::mesh::mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs) : positions(std::move(positions)),
                                                                                uvs(std::move(uvs)) {

}

omc::mesh::mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> uvs, std::vector<uint16_t> indices)
        : positions(std::move(positions)),
          uvs(std::move(uvs)),
          indices(std::move(indices)) {

}

omc::mesh::mesh(std::vector<glm::vec3> positions, std::vector<uint16_t> indices): positions(std::move(positions)),
                                                                                         indices(std::move(indices)) {

}

//
// Created by MED45 on 17.01.2020.
//

#ifndef OPENMC_MESH_H
#define OPENMC_MESH_H

#include <vector>

#include <glm/glm.hpp>

namespace omc {
    class mesh {
    public:
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<uint16_t> indices;

        mesh() = default;

        explicit mesh(std::vector<glm::vec3> positions);

        mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uvs);

        mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> uvs, std::vector<uint16_t> indices);

        mesh(std::vector<glm::vec3> positions, std::vector<uint16_t> indices);

//        void add_vertices(const std::vector<glm::vec3> &positions);
//
//        void add_vertices(const std::vector<glm::vec3> &positions, std::vector<glm::vec2> uvs);
//
//        void add_vertices(const std::vector<glm::vec3> &positions, std::vector<glm::vec2> uvs,
//                          std::vector<uint16_t> indices);
//
//        void add_vertices(const std::vector<glm::vec3> &positions, std::vector<uint16_t> indices);

    };

}

#endif //OPENMC_MESH_H

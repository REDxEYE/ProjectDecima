//
// Created by MED45 on 20.01.2020.
//

#include <iostream>
#include <utility>

#include <stb_image.h>

#include <object.h>

omc::FileTexture::FileTexture(const std::string& path) {
    this->texture_id = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &this->texture_id);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    uint8_t* data = stbi_load(path.c_str(), &dim[0], &dim[1], &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dim[0], dim[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        name = path;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}


const GLuint& omc::FileTexture::id() const {
    return this->texture_id;
}

void omc::FileTexture::bind() {
    glBindTextureUnit(GL_TEXTURE0, this->texture_id);
}

void omc::FileTexture::unbind() {
    glBindTextureUnit(GL_TEXTURE0, 0);
}


omc::MemoryTexture::MemoryTexture(std::string name, glm::ivec2 dim, GLint format, uint8_t channels)
        : name(std::move(name)), dim(dim),channels(channels),format(format) {
    texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture_data.resize(dim[0] * dim[1] * channels);
    glTexImage2D(GL_TEXTURE_2D, 0, format, dim[0], dim[1], 0, format, GL_UNSIGNED_BYTE, texture_data.data());
}

const GLuint& omc::MemoryTexture::id() const {
    return this->texture_id;
}

void omc::MemoryTexture::bind() {
    glBindTextureUnit(GL_TEXTURE0, this->texture_id);
}

void omc::MemoryTexture::unbind() {
    glBindTextureUnit(GL_TEXTURE0, 0);
}

void omc::MemoryTexture::update_texture() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width(), height(), format, GL_UNSIGNED_BYTE, texture_data.data());
    glBindTexture(GL_TEXTURE_2D, 0);

}

//template<typename T>
//T omc::MemoryTexture::get_pixel(uint32_t x, uint32_t y) {
//    uint8_t z = sizeof(T);
//    return texture_data[0 + x * z + y * z * dim[0]];
//}

#ifndef OPENMC_OBJECT_H
#define OPENMC_OBJECT_H

#include <glad/glad.h>

#include <string>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace omc {
    template<typename T>
    class Object {
    public:
        [[nodiscard]] virtual const T& id() const = 0;

        virtual void bind() = 0;

        virtual void unbind() = 0;
    };

    class Shader : public Object<GLuint> {
    private:
        GLuint program_id = 0;
    public:
        Shader(const std::string& vertex_shader, const std::string& fragment_shader, bool is_path);

        [[nodiscard]] inline const GLuint& id() const override {
            return this->program_id;
        };

        inline void bind() override {
            glUseProgram(this->program_id);
        }

        inline void unbind() override {
            glUseProgram(0);
        }

        inline ~Shader() {
            glDeleteProgram(this->program_id);
        };

        [[nodiscard]] inline GLuint uniform_location(const std::string& name) const {
            return glGetUniformLocation(this->program_id, name.c_str());
        }

        inline void uniform_mat4(const std::string& name, const glm::mat4& value) {
            glUniformMatrix4fv(this->uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
        }
    };

    class FileTexture : public Object<GLuint> {
    private:
        GLuint texture_id;
        std::string name;
        glm::ivec2 dim = {0, 0};
    public:
        explicit FileTexture(const std::string& path);

        [[nodiscard]] const GLuint& id() const override;

        void bind() override;

        void unbind() override;
    };

    class MemoryTexture : public Object<GLuint> {
    public:
        typedef std::array<uint8_t, 1> R;
        typedef std::array<uint8_t, 2> RG;
        typedef std::array<uint8_t, 3> RGB;
        typedef std::array<uint8_t, 4> RGBA;
    private:
        GLuint texture_id;
        std::string name;
        glm::ivec2 dim = {0, 0};
        std::vector<uint8_t> texture_data;
        uint32_t channels;
        GLint format;

    public:
        explicit MemoryTexture(std::string name, glm::ivec2 dim, GLint format, uint8_t channels);

        void update_texture();

        template<typename T>
        void set_pixel(uint32_t x, uint32_t y, T color) {
            uint8_t z = sizeof(T);
            for (uint8_t i = 0; i < z; i++) {
                texture_data[i + x * channels + y * channels * dim.x] = color[i];
            }
        }

        template<typename T>
        T get_pixel(uint32_t x, uint32_t y) {
            uint8_t z = sizeof(T);
            return *((T*) &texture_data[0 + x * z + y * z * dim.x]);
        }

        [[nodiscard]] const GLuint& id() const override;

        inline uint32_t width() const { return dim.x; }

        inline uint32_t height() const { return dim.y; }

        inline glm::vec2 size() {return dim;};

        void bind() override;

        void unbind() override;
    };

    class Model : public Object<GLuint> {
    public:
        virtual void draw() = 0;
    };


    class ModelIndexed : public Model {
    private:
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ibo = 0;
        GLuint index_count;
    public:
        ModelIndexed(const std::vector<glm::vec3>& vertices, const std::vector<glm::ivec3>& indices);

        [[nodiscard]] inline const GLuint& id() const override {
            return this->vao;
        }

        inline void bind() override {
            glBindVertexArray(this->vao);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
        }

        inline void unbind() override {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        inline ~ModelIndexed() {
            glDeleteBuffers(1, &this->vbo);
            glDeleteBuffers(1, &this->ibo);
            glDeleteVertexArrays(1, &this->vao);
        }

        void draw() override;
    };
}

#endif //OPENMC_OBJECT_H

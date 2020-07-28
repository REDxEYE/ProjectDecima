#include <object.h>
#include <fstream>

omc::Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader, bool is_path) {
    const auto shader_read = [](const std::string& path) {
        std::string buffer;
        std::string line;

        std::ifstream handle(path);

        if (!handle.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }

        while (std::getline(handle, line)) {
            buffer += line;
            buffer += '\n';
        }

        handle.close();
        return buffer;
    };

    const auto shader_compile = [](const std::string& source, GLenum type) {
        auto source_raw = source.c_str();
        auto shader = glCreateShader(type);

        glShaderSource(shader, 1, &source_raw, nullptr);
        glCompileShader(shader);

        return shader;
    };

    const auto shader_verify = [](GLuint id) {
        int status;
        char buffer[1024];

        glGetProgramiv(id, GL_LINK_STATUS, &status);

        if (!status) {
            glGetProgramInfoLog(id, sizeof(buffer), nullptr, buffer);
            throw std::runtime_error("Error linking shader program: " + std::string(buffer));
        }
    };


    this->program_id = glCreateProgram();

    GLuint vertex_shader_id = shader_compile(is_path ? shader_read(vertex_shader) : vertex_shader, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = shader_compile(is_path ? shader_read(fragment_shader) : fragment_shader, GL_FRAGMENT_SHADER);

    glAttachShader(this->program_id, vertex_shader_id);
    glAttachShader(this->program_id, fragment_shader_id);

    glLinkProgram(this->program_id);
    shader_verify(this->program_id);

    glValidateProgram(this->program_id);
    shader_verify(this->program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

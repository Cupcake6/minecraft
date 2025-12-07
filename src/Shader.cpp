#include "Shader.hpp"

#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdint>
#include <iostream>

static constexpr size_t info_log_size = 512;

Shader::Shader(Type type) {
    shader_handle = glCreateShader(static_cast<GLenum>(type));
    if (!shader_handle) {
        throw std::runtime_error("failed to create shader");
    }
}

Shader::~Shader() noexcept {
    if (shader_handle != 0) {
        glDeleteShader(shader_handle);
    }
}

void Shader::load_from_file(std::string_view filename_string_view) {
    std::string filename(filename_string_view);

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open shader source file '" + filename + '\'');
    }

    std::streamsize size = file.tellg();
    file.seekg(std::ios::beg);

    std::string buffer(size, '\0');
    file.read(buffer.data(), size);
    file.close();

    const char* source = buffer.c_str();

    glShaderSource(shader_handle, 1, &source, NULL);
    glCompileShader(shader_handle);

    GLchar info_log[info_log_size];
    GLsizei info_log_length = 0;
    glGetShaderInfoLog(shader_handle, info_log_size, &info_log_length, info_log);

    if (info_log_length != 0) {
        std::cerr << info_log << std::endl;
    }

    GLint success;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        throw std::runtime_error("failed to compile shader '" + filename + '\'');
    }
}

GLuint Shader::release() {
    GLuint temp = shader_handle;
    shader_handle = 0;
    return temp;
}
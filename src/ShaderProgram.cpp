#include "ShaderProgram.hpp"

#include <stdexcept>
#include <cstdint>
#include <iostream>

static constexpr uint32_t info_log_size = 512;

ShaderProgram::ShaderProgram() {
    shader_program_handle = glCreateProgram();
    if (!shader_program_handle) {
        throw std::runtime_error("failed to create shader program");
    }
}

ShaderProgram::~ShaderProgram() noexcept {
    if (shader_program_handle != 0) {
        glDeleteProgram(shader_program_handle);
    }
}

void ShaderProgram::attach_shader(Shader&& shader) {
    GLuint shader_handle = shader.release();
    glAttachShader(shader_program_handle, shader_handle);
    glDeleteShader(shader_handle);
}

void ShaderProgram::link() {
    glLinkProgram(shader_program_handle);

    GLchar info_log[info_log_size];
    GLsizei info_log_length = 0;
    glGetProgramInfoLog(shader_program_handle, info_log_size, &info_log_length, info_log);

    if (info_log_length != 0) {
        std::cerr << info_log << std::endl;
    }

    GLint success;
    glGetProgramiv(shader_program_handle, GL_LINK_STATUS, &success);
    if (!success) {
        throw std::runtime_error("failed to link shader program");
    }
}

void ShaderProgram::use() {
    glUseProgram(shader_program_handle);
}
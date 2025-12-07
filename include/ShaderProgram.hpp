#pragma once

#include <initializer_list>

#include "gfx.hpp"
#include "Shader.hpp"

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram() noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void attach_shader(Shader&& shader);
    void link();
    void use();

    GLint get_handle() { return shader_program_handle; }

private:
    GLuint shader_program_handle = 0;
};
#pragma once

#include "gfx.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "math/Matrix.hpp"

class Renderer {
private:
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint EBO = 0;
    Shader vertex_shader = Shader(Shader::Type::Vertex);
    Shader fragment_shader = Shader(Shader::Type::Fragment);
    ShaderProgram shader_program = ShaderProgram();
    math::Vector3f view_position;

public:
    Renderer();
    ~Renderer() noexcept;
    void draw();
};
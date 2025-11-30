#pragma once

#include "gfx.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

class Game {
private:
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint EBO = 0;
    Shader vertexShader = Shader(Shader::Type::Vertex);
    Shader fragmentShader = Shader(Shader::Type::Fragment);
    ShaderProgram shaderProgram = ShaderProgram();

public:
    Game();
    ~Game() noexcept;
    void update();
};
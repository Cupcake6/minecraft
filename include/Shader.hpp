#pragma once

#include <string_view>

#include "gfx.hpp"

class Shader {
public:
    enum class Type {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER 
    };

    Shader(Type type);
    ~Shader() noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void load_from_file(std::string_view filename);
    GLuint release();

private:
    GLuint shader_handle = 0;
};
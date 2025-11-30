#include "Game.hpp"

#include <cstddef>

struct Vertex {
    GLfloat position[3];
    GLfloat color[4];
};

Game::Game() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    Vertex vertices[] = {
        (Vertex) {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        (Vertex) {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        (Vertex) {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        (Vertex) {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertexShader.load_from_file("../shaders/vertex.glsl");
    fragmentShader.load_from_file("../shaders/fragment.glsl");
    shaderProgram.attach_shader(std::move(vertexShader));
    shaderProgram.attach_shader(std::move(fragmentShader));
    shaderProgram.link();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Game::~Game() noexcept {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Game::update() {
    glClearColor(0.1f, 0.15f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


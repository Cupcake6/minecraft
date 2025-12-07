#include "Renderer.hpp"

#include <cstddef>

#include "math/Matrix.hpp"
#include "math/pi.hpp"

struct Vertex {
    GLfloat position[3];
    GLfloat color[4];
};

Renderer::Renderer() {
    glClearColor(0.1f, 0.15f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    Vertex vertices[] = {
        (Vertex) {{-1.0f, -1.0f, 3.0f}, {0.0f, 0.0f, 0.0f, 1.0f}},
        (Vertex) {{1.0f, -1.0f, 3.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        (Vertex) {{-1.0f, 1.0f, 3.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        (Vertex) {{-1.0f, -1.0f, 5.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        (Vertex) {{1.0f, 1.0f, 3.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
        (Vertex) {{1.0f, -1.0f, 5.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
        (Vertex) {{-1.0f, 1.0f, 5.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
        (Vertex) {{1.0f, 1.0f, 5.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader.load_from_file("../shaders/vertex.glsl");
    fragment_shader.load_from_file("../shaders/fragment.glsl");
    shader_program.attach_shader(std::move(vertex_shader));
    shader_program.attach_shader(std::move(fragment_shader));
    shader_program.link();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint elements[] = {
        0, 1, 5,
        0, 3, 5,
        0, 1, 4,
        0, 2, 4,
        0, 3, 6,
        0, 2, 6,
        1, 5, 7,
        1, 4, 7,
        2, 4, 7,
        2, 6, 7,
        3, 5, 7,
        3, 6, 7
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

Renderer::~Renderer() noexcept {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Renderer::draw() {
    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);

    double mouse_x, mouse_y;
    glfwGetCursorPos(glfwGetCurrentContext(), &mouse_x, &mouse_y);
    mouse_x = 2.0 * (mouse_x / framebuffer_width) - 1.0;
    mouse_y = 1.0 - 2.0 * (mouse_y / framebuffer_height);

    math::Vector4f movement;
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS) {
        movement.z() += 1.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS) {
        movement.z() -= 1.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS) {
        movement.x() += 1.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS) {
        movement.x() -= 1.0f;
    }

    math::Vector4f direction = math::rotation_y(mouse_x) * movement.normalize();
    if (!movement.is_zero()) {
        view_position += math::Vector3f(direction.x(), 0.0f, direction.z()) * 0.08f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        view_position.y() += 0.08f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        view_position.y() -= 0.08f;
    }

    math::Matrix4f translation = math::translation(
        -view_position.x(), -view_position.y(), -view_position.z()
    );

    math::Matrix4f rotation_y = math::rotation_y(-mouse_x);
    math::Matrix4f rotation_x = math::rotation_x(-mouse_y);

    math::Matrix4f projection = math::perspective_projection(
        math::pi<float>() / 2.0f,
        (float)framebuffer_width / (float)framebuffer_height,
        0.1f,
        100.0f
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader_program.get_handle(), "u_projection"),
        1,
        GL_FALSE,
        (projection * rotation_x * rotation_y * translation).get_flat_data().data()
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_program.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


#include "Game.hpp"

#include <stdexcept>
#include <cassert>

#include "Renderer.hpp"

void Game::create_glfw_window() {
    assert(glfw_window == nullptr);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "minecraft", NULL, NULL);
    if (!glfw_window) {
        throw std::runtime_error("failed to create GLFW window");
    }
}

void Game::loop() {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    create_glfw_window();
    glfwMakeContextCurrent(glfw_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("failed to load GLAD");
    }

    glfwSwapInterval(1);
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    {
        Renderer renderer;
        while (!glfwWindowShouldClose(glfw_window)) {
            glfwPollEvents();
            renderer.draw();
            glfwSwapBuffers(glfw_window);
        }
    }
    
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}
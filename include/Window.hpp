#pragma once

#include <cstdint>

#include "gfx.hpp"

class Window {
private:
    GLFWwindow* glfw_window = nullptr;
    void create_glfw_window();

    static constexpr uint32_t WIDTH = 1200;
    static constexpr uint32_t HEIGHT = 800;

public:
    void loop();
};
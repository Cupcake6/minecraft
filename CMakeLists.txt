cmake_minimum_required(VERSION 3.15)

project(minecraft)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (UNIX)
    find_package(OpenGL REQUIRED)
endif()

add_executable(minecraft
    "src/main.cpp"
    "src/glad.c"
    "src/Window.cpp"
    "src/Game.cpp"
    "src/Shader.cpp"
    "src/ShaderProgram.cpp"
)

add_subdirectory("external/glfw")

target_include_directories(minecraft PRIVATE "include")
target_link_libraries(minecraft PRIVATE glfw)

if (WIN32)
    target_link_libraries(minecraft PRIVATE opengl32)
elseif (UNIX)
    target_link_libraries(minecraft PRIVATE OpenGL::GL)
endif()

// renderer.cpp
#include "../include/graphics/renderer.h"
#include "glad/glad.h"
#include <iostream>

Renderer::Renderer(int width, int height) : width_(width), height_(height), window_(nullptr) {}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width_, height_, "Maze Game", nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return 0;
    }
    glViewport(0, 0, width_, height_);
    return true;
}

void Renderer::shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

void Renderer::clear() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::swapBuffers() {
    glfwSwapBuffers(window_);
}

bool Renderer::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Renderer::pollEvents() {
    glfwPollEvents();
}
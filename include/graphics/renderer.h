// renderer.h
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    bool init();
    void shutdown();
    void clear();
    void swapBuffers();
    bool shouldClose() const;
    void pollEvents();
    GLFWwindow* getWindow() const { return window_; }

private:
    int width_;
    int height_;
    GLFWwindow* window_;
};
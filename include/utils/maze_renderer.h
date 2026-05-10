#pragma once
#include "../include/core/grid.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class MazeRenderer {
public:
    MazeRenderer(int cellSize = 40);
    ~MazeRenderer();

    bool init(int screenWidth, int screenHeight);
    void render(const Grid& grid);
    void cleanup();

private:
    void drawWall(float x1, float y1, float x2, float y2);
    void drawCell(int x, int y, int grid_h, const Cell& cell, float cellW, float cellH);

    bool compileShader(unsigned int& shader, unsigned int type, const char* source);
    bool createShaderProgram();

    int cellSize_;
    float screenWidth_;
    float screenHeight_;
    unsigned int shaderProgram_;
    unsigned int VAO_;
    unsigned int VBO_;
};
#include "../include/utils/maze_renderer.h"
#include <iostream>
#include <vector>
#include <cmath>

static const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

MazeRenderer::MazeRenderer(int cellSize) : cellSize_(cellSize), shaderProgram_(0), VAO_(0), VBO_(0) {
    for (int i = 0; i < 16; ++i) projMatrix_[i] = 0;
}

MazeRenderer::~MazeRenderer() {
    cleanup();
}

bool MazeRenderer::compileShader(unsigned int& shader, unsigned int type, const char* source) {
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool MazeRenderer::createShaderProgram() {
    unsigned int vertexShader, fragmentShader;

    if (!compileShader(vertexShader, GL_VERTEX_SHADER, vertexShaderSource)) return false;
    if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderSource)) return false;

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    int success;
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

void MazeRenderer::setupMatrices(int screenWidth, int screenHeight) {
    projMatrix_[0] = 2.0f / (float)screenWidth;
    projMatrix_[5] = 2.0f / (float)screenHeight;
    projMatrix_[10] = -1.0f;
    projMatrix_[12] = -1.0f;
    projMatrix_[13] = -1.0f;
    projMatrix_[15] = 1.0f;
}

bool MazeRenderer::init(int screenWidth, int screenHeight) {
    if (!createShaderProgram()) return false;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);

    setupMatrices(screenWidth, screenHeight);

    glUseProgram(shaderProgram_);
    int projLoc = glGetUniformLocation(shaderProgram_, "uProjection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix_);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);

    return true;
}

void MazeRenderer::drawWall(float x1, float y1, float x2, float y2) {
    std::vector<float> vertices = { x1, y1, x2, y2 };

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
}

void MazeRenderer::drawCell(int x, int y, const Cell& cell, float cellW, float cellH) {
    float left = x * cellW;
    float right = (x + 1) * cellW;
    float top = y * cellH;
    float bottom = (y + 1) * cellH;

    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    if (cell.top) {
        drawWall(left, top, right, top);
    }
    if (cell.bottom) {
        drawWall(left, bottom, right, bottom);
    }
    if (cell.left) {
        drawWall(left, top, left, bottom);
    }
    if (cell.right) {
        drawWall(right, top, right, bottom);
    }
}

void MazeRenderer::drawEntity(float x, float y, float cellW, float cellH, ) {
    float cx = x * cellW + cellW / 2.0f;
    float cy = y * cellH + cellH / 2.0f;
    float radius = cellW * 0.3f;
    int segments = 20;

    std::vector<float> vertices;
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * i / segments;
        vertices.push_back(cx + radius * cos(angle));
        vertices.push_back(cy + radius * sin(angle));
    }

    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, 0.2f, 0.8f, 0.2f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 1);
}

void MazeRenderer::render(const Grid& grid, const Player& player) {
    float cellW = (float)cellSize_;
    float cellH = (float)cellSize_;

    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);

    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {
            drawCell(x, y, grid.grid[y][x], cellW, cellH);
        }
    }
    drawPlayer((float)player.getX(), (float)player.getY(), cellW, cellH);
}

void MazeRenderer::cleanup() {
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (shaderProgram_) glDeleteProgram(shaderProgram_);
    VAO_ = VBO_ = shaderProgram_ = 0;
}
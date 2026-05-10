#include "../include/utils/maze_renderer.h"
#include <iostream>
#include <vector>
#include <cmath>

static const char* vertexShaderSource = R"(
#version 330 core
in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
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

MazeRenderer::MazeRenderer(int cellSize) : cellSize_(cellSize), shaderProgram_(0), VAO_(0), VBO_(0), screenWidth_(0), screenHeight_(0) {
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
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

bool MazeRenderer::init(int screenWidth, int screenHeight) {
    if (!createShaderProgram()) return false;

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);

    screenWidth_ = (float)screenWidth;
    screenHeight_ = (float)screenHeight;

    return true;
}

void MazeRenderer::drawWall(float x1, float y1, float x2, float y2) {
    float nx1 = (x1 / screenWidth_) * 2.0f - 1.0f;
    float ny1 = 1.0f - (y1 / screenHeight_) * 2.0f;
    float nx2 = (x2 / screenWidth_) * 2.0f - 1.0f;
    float ny2 = 1.0f - (y2 / screenHeight_) * 2.0f;

    float vertices[] = { nx1, ny1, nx2, ny2 };

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
}

void MazeRenderer::drawCell(int x, int y, int grid_h, const Cell& cell, float cellW, float cellH) {
    float left = x * cellW;
    float right = (x + 1) * cellW;
    float top = (grid_h - y) * cellH;
    float bottom = (grid_h - y-1) * cellH;

    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

    

    if (cell.right) {
        drawWall(right, top, right, bottom);
    }
    

    if (cell.bottom) {
        drawWall(left, bottom, right, bottom);
    }
    

   
    
}

void MazeRenderer::render(const Grid& grid) {
    float cellW = (float)cellSize_;
    float cellH = (float)cellSize_;

    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);

    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {
            drawCell(x, y, grid.h, grid.grid[y][x], cellW, cellH);
        }
    }
    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    
}

void MazeRenderer::cleanup() {
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (shaderProgram_) glDeleteProgram(shaderProgram_);
    VAO_ = VBO_ = shaderProgram_ = 0;
}
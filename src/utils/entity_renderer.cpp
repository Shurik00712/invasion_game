#include "../include/utils/entity_renderer.h"
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

EntityRenderer::EntityRenderer(int cellSize) : cellSize_(cellSize), shaderProgram_(0), VAO_(0), VBO_(0) {
    for (int i = 0; i < 16; ++i) projMatrix_[i] = 0;
}

EntityRenderer::~EntityRenderer() {
    cleanup();
}

bool EntityRenderer::compileShader(unsigned int& shader, unsigned int type, const char* source) {
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

bool EntityRenderer::createShaderProgram() {
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

void EntityRenderer::setupMatrices(int screenWidth, int screenHeight) {
    projMatrix_[0] = 2.0f / (float)screenWidth;
    projMatrix_[5] = 2.0f / (float)screenHeight;
    projMatrix_[10] = -1.0f;
    projMatrix_[12] = -1.0f;
    projMatrix_[13] = -1.0f;
    projMatrix_[15] = 1.0f;
}

bool EntityRenderer::init(int screenWidth, int screenHeight) {
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




void EntityRenderer::drawInvader(float x, float y, float r, float g, float b) {
    float cx = x * cellSize_ + cellSize_ / 2.0f;
    float cy = y * cellSize_ + cellSize_ / 2.0f;
    float radius = cellSize_ * 0.3f;
    int segments = 20;

    std::vector<float> vertices;
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * i / segments;
        vertices.push_back(cx + radius * cos(angle));
        vertices.push_back(cy + radius * sin(angle));
    }

    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, r, g, b);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 1);
}
void EntityRenderer::drawPlayer(float x, float y) {
    float cx = x * cellSize_ + cellSize_ / 2.0f;
    float cy = y * cellSize_ + cellSize_ / 2.0f;
    float radius = cellSize_ * 0.3f;
    int segments = 20;

    std::vector<float> vertices;
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * i / segments;
        vertices.push_back(cx + radius * cos(angle));
        vertices.push_back(cy + radius * sin(angle));
    }

    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, 2.f, 2.f, 2.f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 1);
}

void EntityRenderer::render_invader(const Invader& player, float r, float g, float b) {

    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);
    EntityRenderer::drawInvader((float)player.getX(), (float)player.getY(), r, g, b);
}


void EntityRenderer::render_player(const Player& player) {

    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);
    EntityRenderer::drawPlayer((float)player.getX(), (float)player.getY());
}

void EntityRenderer::cleanup() {
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (shaderProgram_) glDeleteProgram(shaderProgram_);
    VAO_ = VBO_ = shaderProgram_ = 0;
}
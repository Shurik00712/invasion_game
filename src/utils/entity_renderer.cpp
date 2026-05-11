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

EntityRenderer::EntityRenderer(int cellSize) : cellSize_(cellSize), shaderProgram_(0), VAO_(0), VBO_(0), animationTime_(0.0f) {
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

void EntityRenderer::setColor(float r, float g, float b) {
    int colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform3f(colorLoc, r, g, b);
}

void EntityRenderer::drawBuffer(const std::vector<float>& vertices, GLenum mode) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(mode, 0, (GLsizei)vertices.size() / 2);
}

void EntityRenderer::drawPlayerBody(float cx, float cy, float size) {
    std::vector<float> bodyVertices = {
        cx - size, cy - size,
        cx + size, cy - size,
        cx + size, cy + size,

        cx - size, cy - size,
        cx + size, cy + size,
        cx - size, cy + size,
    };

    setColor(0.2f, 0.6f, 1.0f);
    drawBuffer(bodyVertices, GL_TRIANGLES);
}

void EntityRenderer::drawPlayerLegs(float cx, float cy, float size, float legOffset) {
    std::vector<float> legVertices = {
        cx - size * 0.5f, cy - size,
        cx - size * 0.3f + legOffset, cy - size - cellSize_ * 0.2f,

        cx + size * 0.3f, cy - size,
        cx + size * 0.5f - legOffset, cy - size - cellSize_ * 0.2f,
    };

    setColor(0.1f, 0.4f, 0.8f);
    drawBuffer(legVertices, GL_LINES);
}

void EntityRenderer::drawPlayerEyes(float cx, float cy, float size) {
    float eyeSize = size * 0.2f;
    float eyeY = cy + size * 0.3f;
    std::vector<float> eyesVertices = {
        cx - size * 0.4f, eyeY + eyeSize,
        cx - size * 0.4f, eyeY - eyeSize,
        cx + size * 0.4f, eyeY + eyeSize,
        cx + size * 0.4f, eyeY - eyeSize,
    };

    setColor(1.0f, 1.0f, 1.0f);
    drawBuffer(eyesVertices, GL_LINES);
}

void EntityRenderer::drawPlayer(float x, float y, float animationPhase) {
    float cx = x * cellSize_ + cellSize_ / 2.0f;
    float cy = y * cellSize_ + cellSize_ / 2.0f;
    float size = cellSize_ * 0.35f;

    float legOffset = sin(animationPhase * 3.0f) * cellSize_ * 0.1f;

    drawPlayerBody(cx, cy, size);
    drawPlayerLegs(cx, cy, size, legOffset);
    drawPlayerEyes(cx, cy, size);
}

void EntityRenderer::drawInvaderUpperBody(float cx, float cy, float size, float r, float g, float b) {
    std::vector<float> upperVertices = {
        cx, cy + size * 1.5f,
        cx - size, cy,
        cx + size, cy,
    };

    setColor(r * 0.8f, g * 0.2f, b * 0.2f);
    drawBuffer(upperVertices, GL_TRIANGLES);
}

void EntityRenderer::drawInvaderLowerBody(float cx, float cy, float size, float r, float g, float b) {
    std::vector<float> lowerVertices = {
        cx, cy - size * 1.5f,
        cx - size, cy,
        cx + size, cy,
    };

    setColor(r * 0.6f, g * 0.1f, b * 0.1f);
    drawBuffer(lowerVertices, GL_TRIANGLES);
}
void EntityRenderer::drawInvaderEyes(float cx, float cy, float size) {
    float eyeY = cy + size * 0.3f;
    float eyeWidth = size * 0.25f;
    float eyeHeight = size * 0.3f;

    std::vector<float> eyeVertices = {
        cx - eyeWidth, eyeY + eyeHeight,
        cx - eyeWidth, eyeY - eyeHeight,
        cx + eyeWidth, eyeY + eyeHeight,

        cx - eyeWidth, eyeY - eyeHeight,
        cx + eyeWidth, eyeY - eyeHeight,
        cx + eyeWidth, eyeY + eyeHeight,
    };

    setColor(1.0f, 0.8f, 0.0f);
    drawBuffer(eyeVertices, GL_TRIANGLES);

    float pupilWidth = eyeWidth * 0.5f;
    float pupilHeight = eyeHeight * 0.5f;
    std::vector<float> pupilVertices = {
        cx - pupilWidth, eyeY + pupilHeight,
        cx - pupilWidth, eyeY - pupilHeight,
        cx + pupilWidth, eyeY + pupilHeight,

        cx - pupilWidth, eyeY - pupilHeight,
        cx + pupilWidth, eyeY - pupilHeight,
        cx + pupilWidth, eyeY + pupilHeight,
    };

    setColor(0.0f, 0.0f, 0.0f);
    drawBuffer(pupilVertices, GL_TRIANGLES);
}

void EntityRenderer::drawInvader(float x, float y, float r, float g, float b, float animationPhase) {
    float cx = x * cellSize_ + cellSize_ / 2.0f;
    float cy = y * cellSize_ + cellSize_ / 2.0f;
    float size = cellSize_ * 0.3f;

    float pulse = 1.0f + sin(animationPhase * 2.0f) * 0.1f;
    float animatedSize = size * pulse;
    float tentacleOffset = sin(animationPhase * 5.0f) * cellSize_ * 0.08f;

    drawInvaderUpperBody(cx, cy, animatedSize, r, g, b);
    drawInvaderLowerBody(cx, cy, animatedSize, r, g, b);
    drawInvaderEyes(cx, cy, animatedSize);
    drawInvaderTentacles(cx, cy, animatedSize, tentacleOffset, r, g, b);
}
void EntityRenderer::drawInvaderTentacles(float cx, float cy, float size, float tentacleOffset, float r, float g, float b) {
    std::vector<float> tentacles = {
        cx - size * 0.5f, cy - size * 0.5f,
        cx - size * 0.7f + tentacleOffset, cy - size * 1.2f,
        cx + size * 0.5f, cy - size * 0.5f,
        cx + size * 0.7f - tentacleOffset, cy - size * 1.2f,
    };

    setColor(r * 0.5f, g * 0.3f, b * 0.3f);
    drawBuffer(tentacles, GL_LINES);
}

void EntityRenderer::render_invaders(std::vector<Invader>& invaders) {
    animationTime_ += 0.016f;

    for (Invader& invader : invaders) {
        render_invader(invader, 1.0f, 0.2f, 0.2f);
    }
}

void EntityRenderer::render_invader(const Invader& invader, float r, float g, float b) {
    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);

    float animPhase = animationTime_ + (invader.getX() + invader.getY()) * 0.5f;
    drawInvader(invader.getVisualX(), invader.getVisualY(), r, g, b, animPhase);
}

void EntityRenderer::render_player(const Player& player) {
    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);

    float animPhase = animationTime_;
    if (player.isMoving()) {
        animPhase *= 2.0f;
    }

    drawPlayer(player.getVisualX(), player.getVisualY(), animPhase);
}
void EntityRenderer::drawCoin(float x, float y, CoinType type, float animationPhase) {
    float cx = x * cellSize_ + cellSize_ / 2.0f;
    float cy = y * cellSize_ + cellSize_ / 2.0f;
    float radius = cellSize_ * 0.2f;

    int segments = 16;
    std::vector<float> circleVertices;
    std::vector<float> innerVertices;

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * i / (float)segments;
        circleVertices.push_back(cx + radius * cos(angle));
        circleVertices.push_back(cy + radius * sin(angle));
    }

    float innerRadius = radius * 0.6f;
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * i / (float)segments;
        innerVertices.push_back(cx + innerRadius * cos(angle));
        innerVertices.push_back(cy + innerRadius * sin(angle));
    }

    char letter;
    switch (type) {
    case CoinType::REBUILD_MAZE:
        setColor(1.0f, 0.5f, 0.0f);
        drawBuffer(circleVertices, GL_TRIANGLE_FAN);
        setColor(1.0f, 0.7f, 0.2f);
        drawBuffer(innerVertices, GL_TRIANGLE_FAN);
        letter = 'R';
        break;

    case CoinType::SPEED_BOOST:
        setColor(0.0f, 0.8f, 1.0f);
        drawBuffer(circleVertices, GL_TRIANGLE_FAN);
        setColor(0.3f, 1.0f, 1.0f);
        drawBuffer(innerVertices, GL_TRIANGLE_FAN);
        letter = 'B';
        break;

    case CoinType::FREEZE_INVADERS:
        setColor(0.5f, 0.5f, 1.0f);
        drawBuffer(circleVertices, GL_TRIANGLE_FAN);
        setColor(0.7f, 0.7f, 1.0f);
        drawBuffer(innerVertices, GL_TRIANGLE_FAN);
        letter = 'F';
        break;

    case CoinType::TELEPORT:
        setColor(0.8f, 0.2f, 0.8f);
        drawBuffer(circleVertices, GL_TRIANGLE_FAN);
        setColor(1.0f, 0.4f, 1.0f);
        drawBuffer(innerVertices, GL_TRIANGLE_FAN);
        letter = 'T';
        break;

    case CoinType::EXTRA_LIFE:
        setColor(0.0f, 1.0f, 0.3f);
        drawBuffer(circleVertices, GL_TRIANGLE_FAN);
        setColor(0.3f, 1.0f, 0.5f);
        drawBuffer(innerVertices, GL_TRIANGLE_FAN);
        letter = 'L';
        break;
    }

    float letterSize = radius * 0.6f;
    std::vector<float> letterVertices;

    switch (letter) {
    case 'R':
        letterVertices = {
            cx - letterSize * 0.4f, cy - letterSize,
            cx - letterSize * 0.4f, cy + letterSize,

            cx - letterSize * 0.4f, cy + letterSize,
            cx + letterSize * 0.2f, cy + letterSize,

            cx + letterSize * 0.2f, cy + letterSize,
            cx + letterSize * 0.4f, cy + letterSize * 0.3f,

            cx + letterSize * 0.4f, cy + letterSize * 0.3f,
            cx + letterSize * 0.2f, cy,

            cx + letterSize * 0.2f, cy,
            cx + letterSize * 0.4f, cy - letterSize * 0.3f,

            cx + letterSize * 0.4f, cy - letterSize * 0.3f,
            cx + letterSize * 0.2f, cy - letterSize,

            cx - letterSize * 0.4f, cy,
            cx + letterSize * 0.2f, cy,
        };
        break;

    case 'B':
        letterVertices = {
            cx - letterSize * 0.4f, cy - letterSize,
            cx - letterSize * 0.4f, cy + letterSize,

            cx - letterSize * 0.4f, cy + letterSize,
            cx + letterSize * 0.2f, cy + letterSize,

            cx + letterSize * 0.2f, cy + letterSize,
            cx + letterSize * 0.4f, cy + letterSize * 0.5f,

            cx + letterSize * 0.4f, cy + letterSize * 0.5f,
            cx + letterSize * 0.2f, cy + letterSize * 0.2f,

            cx - letterSize * 0.4f, cy + letterSize * 0.2f,
            cx + letterSize * 0.2f, cy + letterSize * 0.2f,

            cx + letterSize * 0.2f, cy + letterSize * 0.2f,
            cx + letterSize * 0.4f, cy - letterSize * 0.1f,

            cx + letterSize * 0.4f, cy - letterSize * 0.1f,
            cx + letterSize * 0.2f, cy - letterSize * 0.5f,

            cx + letterSize * 0.2f, cy - letterSize * 0.5f,
            cx + letterSize * 0.4f, cy - letterSize * 0.8f,

            cx + letterSize * 0.4f, cy - letterSize * 0.8f,
            cx + letterSize * 0.2f, cy - letterSize,

            cx - letterSize * 0.4f, cy - letterSize,
            cx + letterSize * 0.2f, cy - letterSize,
        };
        break;

    case 'F':
        letterVertices = {
            cx - letterSize * 0.4f, cy - letterSize,
            cx - letterSize * 0.4f, cy + letterSize,

            cx - letterSize * 0.4f, cy + letterSize,
            cx + letterSize * 0.4f, cy + letterSize,

            cx - letterSize * 0.4f, cy,
            cx + letterSize * 0.2f, cy,
        };
        break;

    case 'T':
        letterVertices = {
            cx - letterSize * 0.4f, cy + letterSize,
            cx + letterSize * 0.4f, cy + letterSize,

            cx, cy + letterSize,
            cx, cy - letterSize,
        };
        break;

    case 'L':
        letterVertices = {
            cx - letterSize * 0.4f, cy + letterSize,
            cx - letterSize * 0.4f, cy - letterSize,

            cx - letterSize * 0.4f, cy - letterSize,
            cx + letterSize * 0.4f, cy - letterSize,
        };
        break;
    }

    setColor(0.0f, 0.0f, 0.0f);
    drawBuffer(letterVertices, GL_LINES);
}

void EntityRenderer::render_coins(std::vector<Coin>& coins) {
    glUseProgram(shaderProgram_);
    glBindVertexArray(VAO_);

    for (const auto& coin : coins) {
        if (coin.active) {
            float animPhase = animationTime_ + (coin.x + coin.y) * 0.5f;
            drawCoin(coin.x, coin.y, coin.type, animPhase);
        }
    }
}
void EntityRenderer::cleanup() {
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (shaderProgram_) glDeleteProgram(shaderProgram_);
    VAO_ = VBO_ = shaderProgram_ = 0;
}
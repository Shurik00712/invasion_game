#pragma once
#include "../include/game/player.h"
#include "../include/game/invader.h"
#include "../include/core/coin.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class EntityRenderer {
public:
    EntityRenderer(int cellSize = 40);
    ~EntityRenderer();

    bool init(int screenWidth, int screenHeight);
    void render_player(const Player& player);
    void render_invader(const Invader& invader, float r, float g, float b);
    void render_invaders(std::vector<Invader>& invaders);
    void render_coins(std::vector<Coin>& coins);
    void cleanup();

private:
    void setColor(float r, float g, float b);
    void drawBuffer(const std::vector<float>& vertices, GLenum mode);

    void drawPlayer(float x, float y, float animationPhase);
    void drawPlayerBody(float cx, float cy, float size);
    void drawPlayerLegs(float cx, float cy, float size, float legOffset);
    void drawPlayerEyes(float cx, float cy, float size);

    void drawInvader(float x, float y, float r, float g, float b, float animationPhase);
    void drawInvaderUpperBody(float cx, float cy, float size, float r, float g, float b);
    void drawInvaderLowerBody(float cx, float cy, float size, float r, float g, float b);
    void drawInvaderEyes(float cx, float cy, float size);
    void drawInvaderTentacles(float cx, float cy, float size, float tentacleOffset, float r, float g, float b);

    void drawCoin(float x, float y, CoinType type, float animationPhase);

    bool compileShader(unsigned int& shader, unsigned int type, const char* source);
    bool createShaderProgram();
    void setupMatrices(int screenWidth, int screenHeight);

    int cellSize_;
    unsigned int shaderProgram_;
    unsigned int VAO_;
    unsigned int VBO_;
    float projMatrix_[16];
    float animationTime_;
};
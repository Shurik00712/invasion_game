#pragma once
#include "../include/game/player.h"
#include "../include/game/invader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class EntityRenderer {
public:
    EntityRenderer(int cellSize = 40);
    ~EntityRenderer();

    bool init(int screenWidth, int screenHeight);
    void render_player(const Player& player);
    void render_invader(const Invader& invader, float r, float g, float b);
    void cleanup();

private:
    void drawPlayer(float x, float y);
    void drawInvader(float x, float y, float r, float g, float b);

    bool compileShader(unsigned int& shader, unsigned int type, const char* source);
    bool createShaderProgram();
    void setupMatrices(int screenWidth, int screenHeight);

    int cellSize_;
    unsigned int shaderProgram_;
    unsigned int VAO_;
    unsigned int VBO_;
    float projMatrix_[16];
};
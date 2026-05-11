#include "../include/core/grid.h"
#include "../include/game/player.h"
#include "../include/game/invader.h"
#include "../include/game/game_logic.h"
#include "../include/algorithms/create_maze.h"
#include "../include/graphics/renderer.h"
#include "../include/utils/maze_renderer.h"
#include "../include/utils/entity_renderer.h"
#include "../include/utils/inputhandler.h"
#include "../include/core/coin.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

int main() {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 640;
    const int CELL_SIZE = 40;

    Grid grid(SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE);
    Player player(1, 1);

    vector<Invader> invaders;
    vector<Coin> coins;
    GameLogic logic;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    MazeRenderer mazeRenderer(CELL_SIZE);
    EntityRenderer entityRenderer(CELL_SIZE);

    auto lastTime = std::chrono::high_resolution_clock::now();
    const double fixedDeltaTime = 1.0 / 60.0;
    double accumulator = 0.0;

    if (!renderer.init()) {
        return -1;
    }

    mazeRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    entityRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetKeyCallback(renderer.getWindow(), InputHandler::keyCallback);

    MazeGenerator::generate(grid);
    MazeGenerator::addExtraPaths(grid, (grid.w * grid.h) / 2);

    logic.addInvader(invaders, 10, 10);
    logic.addInvader(invaders, 15, 5);
    logic.addInvader(invaders, 8, 15);
    logic.addInvader(invaders, 20, 12);

    logic.generateCoins(coins, grid, 15);

    bool running = true;
    while (!renderer.shouldClose() && running) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;

        if (deltaTime > 0.1) {
            deltaTime = 0.1;
        }

        accumulator += deltaTime;

        renderer.pollEvents();

        Command cmd = InputHandler::getCommand(renderer.getWindow());
        if (!logic.update(grid, player, cmd)) {
            running = false;
            break;
        }

        while (accumulator >= fixedDeltaTime) {
            player.updateMovement();
            logic.updateInvaders(invaders, player, grid);
            logic.updateTimers(player);
            accumulator -= fixedDeltaTime;
        }

        player.updateMovement();
        logic.updateInvaders(invaders, player, grid);
        logic.checkCoinCollision(player, coins, grid, invaders);
        logic.updateTimers(player);

        renderer.clear();
        mazeRenderer.render(grid);
        entityRenderer.render_coins(coins);
        entityRenderer.render_invaders(invaders);
        entityRenderer.render_player(player);
        renderer.swapBuffers();

        glfwWaitEventsTimeout(0.016);
    }

    return 0;
}
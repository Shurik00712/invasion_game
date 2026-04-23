// main.cpp
#include "../include/core/grid.h"
#include "../include/game/player.h"
#include "../include/game/invader.h"
#include "../include/game/game_logic.h"
#include "../include/algorithms/create_maze.h"
#include "../include/graphics/renderer.h"
#include "../include/utils/maze_renderer.h"
#include "../include/utils/inputhandler.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 640;
    const int CELL_SIZE = 40;

    Grid grid(SCREEN_WIDTH/CELL_SIZE, SCREEN_HEIGHT/CELL_SIZE);
    Player player(0, 0);
    Invader enemy(0, 10);
    GameLogic logic;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    MazeRenderer mazeRenderer(CELL_SIZE);

    if (!renderer.init()) {
        return -1;
    }

    mazeRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetKeyCallback(renderer.getWindow(), InputHandler::keyCallback);

    MazeGenerator::generate(grid);

    bool running = true;

    while (!renderer.shouldClose() && running) {
        renderer.pollEvents();

        Command cmd = InputHandler::getCommand(renderer.getWindow());
        if (!logic.update(grid, player, cmd)) {
            running = false;
            break;
        }

        renderer.clear();
        mazeRenderer.render(grid, player);
        renderer.swapBuffers();

        glfwWaitEventsTimeout(0.016);
    }

    return 0;
}
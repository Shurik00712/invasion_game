#include "../include/core/grid.h"
#include "../include/game/player.h"
#include "../include/game/invader.h"
#include "../include/game/game_logic.h"
#include "../include/algorithms/create_maze.h"
#include "../include/graphics/renderer.h"
#include "../include/utils/maze_renderer.h"
#include "../include/utils/entity_renderer.h"
#include "../include/utils/inputhandler.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

int main() {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 640;
    const int CELL_SIZE = 40;

    Grid grid(SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE);
    Player player(1, 1);

    vector<Invader> invaders;
    GameLogic logic;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    MazeRenderer mazeRenderer(CELL_SIZE);
    EntityRenderer entityRenderer(CELL_SIZE);

    if (!renderer.init()) {
        return -1;
    }

    mazeRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    entityRenderer.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetKeyCallback(renderer.getWindow(), InputHandler::keyCallback);

    MazeGenerator::generate(grid);
    MazeGenerator::addExtraPaths(grid, (grid.w * grid.h) / 10);

    logic.addInvader(invaders, 10, 10);
    logic.addInvader(invaders, 15, 5);
    logic.addInvader(invaders, 8, 15);
    logic.addInvader(invaders, 20, 12);

    bool running = true;
    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {
            std::cout << grid.grid[y][x].right << " " << grid.grid[y][x].bottom << " | ";
        }
        std::cout << std::endl;
    }
    while (!renderer.shouldClose() && running) {
        renderer.pollEvents();

        Command cmd = InputHandler::getCommand(renderer.getWindow());
        if (!logic.update(grid, player, cmd)) {
            running = false;
            break;
        }

        logic.updateInvaders(invaders, player, grid);

        renderer.clear();
        mazeRenderer.render(grid);
        entityRenderer.render_invaders(invaders);
        entityRenderer.render_player(player);
        renderer.swapBuffers();

        glfwWaitEventsTimeout(10);
    }

    return 0;
}
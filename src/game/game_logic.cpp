// game_logic.cpp
#include "../include/game/game_logic.h"

bool GameLogic::update(Grid& grid, Player& player, Command cmd) {
    switch (cmd) {
    case Command::MOVE_UP:    tryMove(player, grid, 0, -1); return true;
    case Command::MOVE_DOWN:  tryMove(player, grid, 0, 1);  return true;
    case Command::MOVE_LEFT:  tryMove(player, grid, -1, 0); return true;
    case Command::MOVE_RIGHT: tryMove(player, grid, 1, 0);  return true;
    case Command::QUIT:       return false;
    default:                  return true;
    }
}

bool GameLogic::isValidMove(const Player& player, const Grid& grid, int dx, int dy) const {
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    if (newX < 0 || newX >= grid.w || newY < 0 || newY >= grid.h) {
        return false;
    }

    const Cell& current = grid.grid[player.getY()][player.getX()];
    if (dx == -1 && current.left) return false;
    if (dx == 1 && current.right) return false;
    if (dy == -1 && current.top) return false;
    if (dy == 1 && current.bottom) return false;

    return true;
}

void GameLogic::tryMove(Player& player, const Grid& grid, int dx, int dy) {
    if (isValidMove(player, grid, dx, dy)) {
        player.move(dx, dy);
    }
}

void GameLogic::changeWalls(Grid& grid) {
    for (int i = 1; i < grid.h - 1; ++i) {
        for (int j = 1; j < grid.w - 1; ++j) {
            grid.grid[i][j].inverse();
        }
    }
}
#include "../../include/game/game_logic.h"
#include <vector>
#include <iostream>
using namespace std;

bool GameLogic::update(Grid& grid, Player& player, Command cmd) {
    switch (cmd) {
    case Command::MOVE_UP:    tryMove(player, grid, 0, -1); break;
    case Command::MOVE_DOWN:  tryMove(player, grid, 0, 1);  break;
    case Command::MOVE_LEFT:  tryMove(player, grid, -1, 0); break;
    case Command::MOVE_RIGHT: tryMove(player, grid, 1, 0);  break;
    case Command::QUIT:       return false;
    default:                  break;
    }
    return true;
}

bool GameLogic::isValidMove(const Entity& entity, const Grid& grid, int dx, int dy) const {
    int x = entity.getX();
    int y = entity.getY();
    int newX = x + dx;
    int newY = y + dy;

    if (newX < 0 || newX >= grid.w || newY < 0 || newY >= grid.h) {
        return false;
    }

    if (dx == 1) {
        if (grid.grid[y][x].right) {
            return false;
        }
    }
    else if (dx == -1) {
        if (grid.grid[y][x - 1].right) {
            return false;
        }
    }
    else if (dy == 1) {
        if (grid.grid[y][x].bottom) {
            return false;
        }
    }
    else if (dy == -1) {
        if (grid.grid[y - 1][x].bottom) {
            return false;
        }
    }

    return true;
}

void GameLogic::addInvader(vector<Invader>& invaders, int x, int y) {
    invaders.push_back(Invader(x, y));
}

void GameLogic::updateInvaders(vector<Invader>& invaders, Player& player, const Grid& grid) {
    for (Invader& invader : invaders) {
        invader.chase(&player, grid);
        invader.updateMovement();
    }
}

void GameLogic::tryMove(Entity& entity, const Grid& grid, int dx, int dy) {
    if (isValidMove(entity, grid, dx, dy)) {
        entity.move(dx, dy);
    }
}

void GameLogic::changeWalls(Grid& grid) {
    for (int i = 1; i < grid.h - 1; ++i) {
        for (int j = 1; j < grid.w - 1; ++j) {
            grid.grid[i][j].inverse();
        }
    }
}
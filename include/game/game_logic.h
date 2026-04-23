#pragma once
#include "../include/core/grid.h"
#include "player.h"

enum class Command {
    NONE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, QUIT, SHOOT
};

class GameLogic {
public:
    bool update(Grid& grid, Player& player, Command cmd);
    void changeWalls(Grid& grid);

private:
    bool isValidMove(const Player& player, const Grid& grid, int dx, int dy) const;
    void tryMove(Player& player, const Grid& grid, int dx, int dy);
};
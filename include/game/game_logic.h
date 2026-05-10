#pragma once
#include "../core/grid.h"
#include "invader.h"
#include "player.h"

enum class Command {
    NONE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, QUIT, SHOOT
};

class GameLogic {
public:
    bool update(Grid& grid, Player& player, Command cmd);
    void changeWalls(Grid& grid);
    void addInvader(std::vector<Invader>& invaders, int x, int y);
    void updateInvaders(std::vector<Invader>& invaders, Player& player, const Grid& grid);

private:
    bool isValidMove(const Entity& entity, const Grid& grid, int dx, int dy) const;
    void tryMove(Entity& entity, const Grid& grid, int dx, int dy);
};
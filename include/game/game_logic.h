#pragma once
#include "../core/grid.h"
#include "invader.h"
#include "player.h"
#include "../core/coin.h"
#include <vector>

enum class Command {
    NONE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, QUIT, SHOOT
};

class GameLogic {
public:
    GameLogic();
    bool update(Grid& grid, Player& player, Command cmd);
    void changeWalls(Grid& grid);
    void addInvader(std::vector<Invader>& invaders, int x, int y);
    void updateInvaders(std::vector<Invader>& invaders, Player& player, const Grid& grid);

    void generateCoins(std::vector<Coin>& coins, const Grid& grid, int count);
    void checkCoinCollision(Player& player, std::vector<Coin>& coins, Grid& grid, std::vector<Invader>& invaders);
    void updateCoins(std::vector<Coin>& coins);
    void applyCoinEffect(CoinType type, Player& player, Grid& grid, std::vector<Invader>& invaders);
    void updateTimers(Player& player);

private:
    bool isValidMove(const Entity& entity, const Grid& grid, int dx, int dy) const;
    void tryMove(Entity& entity, const Grid& grid, int dx, int dy);
    void rebuildMaze(Grid& grid);

    float freezeTimer_;
    float speedBoostTimer_;
    bool speedBoostActive_;
};
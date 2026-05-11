#include "../../include/game/game_logic.h"
#include "../../include/algorithms/create_maze.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

GameLogic::GameLogic() : freezeTimer_(0.0f), speedBoostTimer_(0.0f), speedBoostActive_(false) {}

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
    if (freezeTimer_ > 0.0f) {
        return;
    }

    for (Invader& invader : invaders) {
        invader.chase(&player, grid);
        invader.updateMovement();
    }
}

void GameLogic::tryMove(Entity& entity, const Grid& grid, int dx, int dy) {
    if (!entity.isMoving() && isValidMove(entity, grid, dx, dy)) {
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

void GameLogic::generateCoins(vector<Coin>& coins, const Grid& grid, int count) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distX(0, grid.w - 1);
    uniform_int_distribution<int> distY(0, grid.h - 1);
    uniform_int_distribution<int> distType(0, 4);

    coins.clear();

    for (int i = 0; i < count; ++i) {
        int x = distX(gen);
        int y = distY(gen);

        if ((x == 1 && y == 1) || (x == 0 && y == 0)) {
            --i;
            continue;
        }

        bool tooClose = false;
        for (const auto& coin : coins) {
            if (abs(coin.x - x) + abs(coin.y - y) < 3) {
                tooClose = true;
                break;
            }
        }

        if (tooClose) {
            --i;
            continue;
        }

        CoinType type = static_cast<CoinType>(distType(gen));
        coins.push_back(Coin(x, y, type));
    }
}

void GameLogic::checkCoinCollision(Player& player, vector<Coin>& coins, Grid& grid, vector<Invader>& invaders) {
    int playerX = player.getX();
    int playerY = player.getY();

    for (auto& coin : coins) {
        if (coin.active && coin.x == playerX && coin.y == playerY) {
            coin.active = false;
            applyCoinEffect(coin.type, player, grid, invaders);
        }
    }

    coins.erase(
        remove_if(coins.begin(), coins.end(), [](const Coin& c) { return !c.active; }),
        coins.end()
    );
}

void GameLogic::updateCoins(vector<Coin>& coins) {
    for (auto& coin : coins) {
        coin.animationPhase += 0.05f;
    }
}

void GameLogic::applyCoinEffect(CoinType type, Player& player, Grid& grid, vector<Invader>& invaders) {
    random_device rd;
    mt19937 gen(rd());

    switch (type) {
    case CoinType::REBUILD_MAZE:
        rebuildMaze(grid);
        break;

    case CoinType::SPEED_BOOST:
        if (!speedBoostActive_) {
            player.setSpeed(player.getSpeed() * 1.5f);
            speedBoostActive_ = true;
            speedBoostTimer_ = 5.0f;
        }
        break;

    case CoinType::FREEZE_INVADERS:
        freezeTimer_ = 3.0f;
        break;

    case CoinType::TELEPORT:
    {
        uniform_int_distribution<int> distX(1, grid.w - 2);
        uniform_int_distribution<int> distY(1, grid.h - 2);
        for (auto& invader : invaders) {
            invader.setPosition(distX(gen), distY(gen));
        }
    }
    break;

    case CoinType::EXTRA_LIFE:
        break;
    }
}

void GameLogic::updateTimers(Player& player) {
    if (freezeTimer_ > 0.0f) {
        freezeTimer_ -= 0.016f;
    }

    if (speedBoostActive_) {
        speedBoostTimer_ -= 0.016f;
        if (speedBoostTimer_ <= 0.0f) {
            player.setSpeed(0.1f);
            speedBoostActive_ = false;
        }
    }
}

void GameLogic::rebuildMaze(Grid& grid) {
    MazeGenerator::generate(grid);
    MazeGenerator::addExtraPaths(grid, (grid.w * grid.h) / 3);
}
#pragma once
#include <vector>
#include <random>

enum class CoinType {
    REBUILD_MAZE,
    SPEED_BOOST,
    FREEZE_INVADERS,
    TELEPORT,
    EXTRA_LIFE
};

struct Coin {
    int x, y;
    CoinType type;
    bool active;
    float animationPhase;

    Coin(int x, int y, CoinType type) : x(x), y(y), type(type), active(true), animationPhase(0.0f) {}
};
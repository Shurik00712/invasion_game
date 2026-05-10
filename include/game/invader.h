#pragma once
#include "entity.h"
#include "../core/grid.h"
#include <vector>
#include <queue>
#include <map>
#include <unordered_set>

struct VectorHash {
    size_t operator()(const std::pair<int, int>& p) const;
};

class Invader : public Entity {
public:
    Invader(int startX = 0, int startY = 0);
    void move(int dx, int dy) override;
    void updateMovement() override;
    void chase(Entity* target, const Grid& grid);
    void setGrid(const Grid* grid) { grid_ = grid; }

private:
    std::vector<std::pair<int, int>> findPath(const Grid& grid, int targetX, int targetY);
    std::pair<int, int> getNextStep(const Grid& grid, int targetX, int targetY);

    Entity* target_ = nullptr;
    const Grid* grid_ = nullptr;
    std::vector<std::pair<int, int>> currentPath_;
    int pathIndex_ = 0;
};
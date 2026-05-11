// invader.cpp
#include "../../include/game/invader.h"
#include <cmath>
#include <queue>
#include <map>
#include <algorithm>

size_t VectorHash::operator()(const std::pair<int, int>& p) const {
    return p.first * 31 + p.second;
}

Invader::Invader(int startX, int startY) : Entity(startX, startY, 0.05f) {}

void Invader::move(int dx, int dy) {
    Entity::move(dx, dy);
}

void Invader::updateMovement() {
    if (!isMoving_ && !currentPath_.empty() && pathIndex_ < currentPath_.size()) {
        auto& nextPos = currentPath_[pathIndex_];
        int dx = nextPos.first - targetX_;
        int dy = nextPos.second - targetY_;

        if (std::abs(dx) + std::abs(dy) == 1) {
            if (dx != 0) dx = (dx > 0) ? 1 : -1;
            if (dy != 0) dy = (dy > 0) ? 1 : -1;
            move(dx, dy);
            pathIndex_++;
        }
        else {
            currentPath_.clear();
            pathIndex_ = 0;
        }
    }

    if (isMoving_) {
        float dx = targetX_ - visualX_;
        float dy = targetY_ - visualY_;

        if (std::abs(dx) > 0.001f) {
            visualX_ += (dx > 0 ? 1.0f : -1.0f) * speed_;
            if (std::abs(visualX_ - targetX_) < speed_) {
                visualX_ = (float)targetX_;
            }
        }

        if (std::abs(dy) > 0.001f) {
            visualY_ += (dy > 0 ? 1.0f : -1.0f) * speed_;
            if (std::abs(visualY_ - targetY_) < speed_) {
                visualY_ = (float)targetY_;
            }
        }

        if (std::abs(visualX_ - targetX_) < 0.001f && std::abs(visualY_ - targetY_) < 0.001f) {
            visualX_ = (float)targetX_;
            visualY_ = (float)targetY_;
            isMoving_ = false;
        }
    }
}

std::vector<std::pair<int, int>> Invader::findPath(const Grid& grid, int targetX, int targetY) {
    std::vector<std::pair<int, int>> path;

    if (!grid_ || (targetX_ == targetX && targetY_ == targetY)) {
        return path;
    }

    std::queue<std::pair<int, int>> q;
    std::unordered_set<std::pair<int, int>, VectorHash> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    q.push({ targetX_, targetY_ });
    visited.insert({ targetX_, targetY_ });
    parent[{targetX_, targetY_}] = { -1, -1 };

    std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    bool found = false;
    std::pair<int, int> target = { targetX, targetY };

    while (!q.empty() && !found) {
        auto current = q.front();
        q.pop();

        for (const auto& dir : directions) {
            int newX = current.first + dir.first;
            int newY = current.second + dir.second;
            std::pair<int, int> next = { newX, newY };

            if (newX >= 0 && newX < grid.w && newY >= 0 && newY < grid.h &&
                visited.find(next) == visited.end()) {

                bool canPass = false;
                int x = current.first;
                int y = current.second;

                if (dir.first == 1) {
                    canPass = !grid.grid[y][x].right;
                }
                else if (dir.first == -1) {
                    canPass = !grid.grid[y][x - 1].right;
                }
                else if (dir.second == 1) {
                    canPass = !grid.grid[y][x].bottom;
                }
                else if (dir.second == -1) {
                    canPass = !grid.grid[y - 1][x].bottom;
                }

                if (canPass) {
                    visited.insert(next);
                    parent[next] = current;
                    q.push(next);

                    if (next == target) {
                        found = true;
                        break;
                    }
                }
            }
        }
    }
    if (found) {
        std::pair<int, int> current = target;
        while (current != std::pair<int, int>{targetX_, targetY_}) {
            path.push_back(current);
            current = parent[current];
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}

std::pair<int, int> Invader::getNextStep(const Grid& grid, int targetX, int targetY) {
    auto path = findPath(grid, targetX, targetY);
    if (!path.empty()) {
        return path[0];
    }
    return { targetX_, targetY_ };
}

void Invader::chase(Entity* target, const Grid& grid) {
    if (!target) return;
    target_ = target;
    grid_ = &grid;

    currentPath_ = findPath(grid, target->getX(), target->getY());
    pathIndex_ = 0;
}
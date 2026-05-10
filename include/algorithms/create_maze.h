#pragma once
#include "../core/grid.h"
#include <vector>
#include <utility>
#include <random>

class MazeGenerator {
public:
    static void generate(Grid& grid);
    static void addExtraPaths(Grid& grid, int extraPathsCount);

private:
    using Point = std::pair<int, int>;
    static void shuffleDirections(std::vector<Point>& directions);
    static void removeWall(Grid& grid, const Point& current, const Point& direction);
    static void removeWallBetween(Grid& grid, const Point& a, const Point& b);
    static bool isInside(const Point& p, int width, int height);
    static std::mt19937 gen;
};
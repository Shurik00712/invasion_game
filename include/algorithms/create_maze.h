// create_maze.h
#pragma once
#include "../include/core/grid.h"
#include <vector>
#include <utility>

class MazeGenerator {
public:
    static void generate(Grid& grid);

private:
    using Point = std::pair<int, int>;
    static void shuffleDirections(std::vector<Point>& directions);
    static void removeWall(Grid& grid, const Point& current, const Point& direction);
    static bool isInside(const Point& p, int width, int height);
};
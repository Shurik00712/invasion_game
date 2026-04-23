// create_maze.cpp
#include "../include/algorithms/create_maze.h"
#include <deque>
#include <random>
#include <algorithm>

void MazeGenerator::generate(Grid& grid) {
    std::deque<Point> stack;
    stack.push_back({ 0, 0 });

    std::vector<Point> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
    std::vector<std::vector<bool>> visited(grid.h, std::vector<bool>(grid.w, false));
    visited[0][0] = true;

    while (!stack.empty()) {
        Point current = stack.back();
        stack.pop_back();

        shuffleDirections(directions);

        for (const auto& dir : directions) {
            Point next = { current.first + dir.first, current.second + dir.second };

            if (isInside(next, grid.w, grid.h) && !visited[next.second][next.first]) {
                removeWall(grid, current, dir);
                visited[next.second][next.first] = true;
                stack.push_back(next);
            }
        }
    }
}

void MazeGenerator::shuffleDirections(std::vector<Point>& directions) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(directions.begin(), directions.end(), gen);
}

void MazeGenerator::removeWall(Grid& grid, const Point& current, const Point& direction) {
    auto& cell = grid.grid[current.second][current.first];

    if (direction.first == -1) {
        cell.left = false;
        grid.grid[current.second][current.first - 1].right = false;
    }
    else if (direction.first == 1) {
        cell.right = false;
        grid.grid[current.second][current.first + 1].left = false;
    }
    else if (direction.second == -1) {
        cell.top = false;
        grid.grid[current.second - 1][current.first].bottom = false;
    }
    else if (direction.second == 1) {
        cell.bottom = false;
        grid.grid[current.second + 1][current.first].top = false;
    }
}

bool MazeGenerator::isInside(const Point& p, int width, int height) {
    return p.first >= 0 && p.first < width && p.second >= 0 && p.second < height;
}
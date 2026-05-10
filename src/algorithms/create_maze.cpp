#include "../../include/algorithms/create_maze.h"
#include <deque>
#include <algorithm>
#include <random>

std::mt19937 MazeGenerator::gen(std::random_device{}());

void MazeGenerator::generate(Grid& grid) {
    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {
            grid.grid[y][x].right = true;
            grid.grid[y][x].bottom = true;
        }
    }

    std::deque<Point> stack;
    stack.push_back({0, 0});

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
                if (dir.first == 1) {
                    grid.grid[current.second][current.first].right = false;
                }
                else if (dir.first == -1) {
                    grid.grid[next.second][next.first].right = false;
                }
                else if (dir.second == 1) {
                    grid.grid[current.second][current.first].bottom = false;
                }
                else if (dir.second == -1) {
                    grid.grid[next.second][next.first].bottom = false;
                }

                visited[next.second][next.first] = true;
                stack.push_back(next);
            }
        }
    }

}

void MazeGenerator::addExtraPaths(Grid& grid, int extraPathsCount) {
    std::uniform_int_distribution<int> distX(1, grid.w - 2);
    std::uniform_int_distribution<int> distY(1, grid.h - 2);
    std::uniform_int_distribution<int> distDir(0, 3);

    std::vector<Point> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    for (int i = 0; i < extraPathsCount; ++i) {
        int x = distX(gen);
        int y = distY(gen);
        auto& dir = directions[distDir(gen)];

        Point neighbor = { x + dir.first, y + dir.second };

        if (isInside(neighbor, grid.w, grid.h)) {
            if (dir.first == 1) {
                grid.grid[y][x].right = false;
            }
            else if (dir.first == -1) {
                grid.grid[y][x - 1].right = false;
            }
            else if (dir.second == 1) {
                grid.grid[y][x].bottom = false;
            }
            else if (dir.second == -1) {
                grid.grid[y - 1][x].bottom = false;
            }
        }
    }
}

void MazeGenerator::shuffleDirections(std::vector<Point>& directions) {
    std::shuffle(directions.begin(), directions.end(), gen);
}

bool MazeGenerator::isInside(const Point& p, int width, int height) {
    return p.first >= 0 && p.first < width && p.second >= 0 && p.second < height;
}
#pragma once
#include <vector>
#include "cell.h"
struct Grid {
	int w, h;
	std::vector<std::vector<Cell>> grid;
	Grid(int w, int h) : w(w), h(h), grid(h, std::vector<Cell>(w, Cell())) {}
	//Cell& operator
};
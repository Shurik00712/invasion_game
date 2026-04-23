struct Cell {
	bool left, right, bottom, top;
	Cell(bool left = 1, bool right = 1, bool bottom = 1, bool top = 1) :
		left(left), right(right), bottom(bottom), top(top) {}
	void inverse() {
		left ^= 1;
		right ^= 1;
		bottom ^= 1;
		top ^= 1;
	}
};
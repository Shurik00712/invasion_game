struct Cell {
	bool right, bottom;
	Cell(bool right = true, bool bottom = true) :
		right(right), bottom(bottom) {
	}
	void inverse() {
		right ^= 1;
		bottom ^= 1;
	}
};
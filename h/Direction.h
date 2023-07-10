#pragma once
#include "Board.h"
class Direction {
public:
	static const int SECONDBEST = -1;
	static const int NONE = 0;
	static const int LEFT  = 1;
	static const int RIGHT = Board::BOARDSIZE - 1;
	static const int FRONT = Board::BOARDSIZE / 2;
	

	static int left(int cell_number);
	static int right(int cell_number);
	static int front(int cell_number);
	static int calc(int cell_number, int direction);
	static int calcDirection(int from_cell, int to_cell);
};

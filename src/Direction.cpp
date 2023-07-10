#include "Direction.h"

int Direction::left(int cell_number) {
	return (cell_number + LEFT) % Board::BOARDSIZE;
}

int Direction::right(int cell_number) {
	return (cell_number + RIGHT) % Board::BOARDSIZE;
}

int Direction::front(int cell_number) {
	return (cell_number + FRONT) % Board::BOARDSIZE;
}

int Direction::calc(int cell_number, int direction) {
	return (cell_number + direction) % Board::BOARDSIZE;
}

int Direction::calcDirection(int from_cell, int to_cell) {
	return (Board::BOARDSIZE + to_cell - from_cell) % Board::BOARDSIZE;
}

#include "Cell.h"

Cell::Cell() {
	num = 0;
}

Cell::Cell(Cell& cl) {
	num = cl.size();
	for (int i = 0; i < HEIGHT_MAX; ++i) {
		cell[i] = cl.get(i);
	}
}

bool Cell::add(const Piece& piece) {
	if (num >= HEIGHT_MAX) return false;

	cell[num] = piece;
	num++;

	return true;
}

bool Cell::canPut() {
	return num < HEIGHT_MAX;
}

Piece Cell::poll() {
	if (num <= 0) return INVALID_PIECE;
	num--;

	return cell[num];
}

Piece Cell::top() {
	if (num <= 0) return INVALID_PIECE;

	return cell[num - 1];
}

bool Cell::isStackUp(const Piece& piece) {
	if (num != HEIGHT_MAX) return false;
	for (int i = 0; i < HEIGHT_MAX; ++i) {
		if (cell[i].Owner() != piece.Owner()) return false;
	}

	return true;
}

Piece Cell::get(int index) {
	if (index >= num || index < 0) return INVALID_PIECE;

	return cell[index];
}

int Cell::size() {
	return num;
}

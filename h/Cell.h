#pragma once
#include "Piece.h"

class Cell {
public:
	static const int HEIGHT_MAX = 3;

private:
	Piece INVALID_PIECE;
	Piece cell[HEIGHT_MAX];
	int num;

public:
	Cell();
	Cell(Cell& cell);

	bool add(const Piece& piece);
	bool canPut();
	Piece poll();
	Piece top();
	bool isStackUp(const Piece& piece);
	Piece get(int index);
	int size();

};

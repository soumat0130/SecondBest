#include "Piece.h"

Piece::Piece() {
	owner = -1;
}

Piece::Piece(int owner) {
	this->owner = owner;
}

int Piece::Owner() const {
	return this->owner;
}

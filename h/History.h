#pragma once
#include "Piece.h"
class History {
	std::stack<std::tuple<Piece,int, int, bool, bool>> history;

public:
	History();
	History(History& history);
	void add(const Piece& piece, int cell_number, int direction, bool white_secondbest_flag, bool gray_secondbest_flag);
	std::tuple<Piece, int, int, bool, bool> poll();
	std::tuple<Piece, int, int, bool, bool> top();
};

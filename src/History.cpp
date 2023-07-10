#include "History.h"

History::History() {
	
}

History::History(History& his) {
	history = his.history;
}

void History::add(const Piece& piece, int cell_number, int direction, bool white_secondbest_flag, bool gray_secondbest_flag) {
	history.push(std::make_tuple(piece, cell_number, direction, white_secondbest_flag, gray_secondbest_flag));
}

std::tuple<Piece, int, int, bool, bool> History::poll() {
	std::tuple<Piece, int, int, bool, bool> tuple = history.top();
	history.pop();

	return tuple;
}

std::tuple<Piece, int, int, bool, bool> History::top() {
	return history.top();
}

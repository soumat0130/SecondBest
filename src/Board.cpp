#include "Board.h"
#include "Direction.h"

Board::Board() {
	turn = 0;
	piece_num = 0;
	isSecondBestFlag[0] = true;
	isSecondBestFlag[1] = false;
}

Board::Board(Board& board) {
	turn = board.turn;
	piece_num = board.piece_num;
	isSecondBestFlag[0] = board.isSecondBestFlag[0];
	isSecondBestFlag[1] = board.isSecondBestFlag[1];
	std::tuple<Piece, int, int> tuple = board.getSecondBestTuple();
	secondBestTuple = std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		cells[i] = board.get(i);
	}

	history = board.history;

}

bool Board::isChain(const Piece& piece) {
	int count = 0;
	int half = BOARDSIZE >> 1;

	for (int i = 0; i < BOARDSIZE + half; ++i) {
		int j = i % BOARDSIZE;

		if (cells[j].top().Owner() == piece.Owner()) count++;
		else count = 0;

		if (count == half) return true;
	}

	return false;
}

bool Board::isStackUp(const Piece& piece) {
	for (int i = 0; i < BOARDSIZE; ++i) {
		if (cells[i].isStackUp(piece)) return true;
	}

	return false;
}

bool Board::isImmovable(const Piece& piece) {
	if (isPutTime()) return false;

	for (int i = 0; i < BOARDSIZE; ++i) {
		if (cells[i].top().Owner() != piece.Owner()) continue;
		if (canMove(i)) return false;
	}

	return true;
}

bool Board::isFinish(const Piece& piece) {
	// if (canSecondBest()) return false;

	Piece opponent(1 - piece.Owner());
	return isChain(opponent) || isStackUp(opponent) || isImmovable(piece);
}

bool Board::isFinish() {
	Piece white(Piece::WHITE);
	Piece gray(Piece::GRAY);
	return isFinish(white) || isFinish(gray);
}

// 2...Draw
// 1...Gray
// 0...White
// -1..false
int Board::winner() {
	Piece white(Piece::WHITE);
	Piece gray(Piece::GRAY);
	bool gray_win = isFinish(white);
	bool white_win = isFinish(gray);
	if (white_win && gray_win) {
		if (canSecondBest()) return -1;
		else return 2;
	}
	else if (gray_win) {
		if (getNowPiece().Owner() == Piece::WHITE && canSecondBest()) return -1;
		else return Piece::GRAY;
	}
	else if (white_win) {
		if (getNowPiece().Owner() == Piece::GRAY && canSecondBest()) return -1;
		else return Piece::WHITE;
	}
	else return -1;
}

int Board::chainNum(const Piece& piece) {
	int count = 0;
	int half = BOARDSIZE >> 1;
	int res = 0;
	for (int i = 0; i < BOARDSIZE + half; ++i) {
		int j = i % BOARDSIZE;

		if (cells[j].top().Owner() == piece.Owner()) count++;
		else count = 0;

		res = std::max(res, count);
		if (count == half) return res;
	}

	return res;
}

int Board::stackNum(const Piece& piece, int index) {
	int res = 0;
	for (int i = 0; i < cells[index].size(); ++i) {
		if (cells[index].get(i).Owner() == piece.Owner()) res++;
		else res = 0;
	}

	return res;
}

// 未完成
Array<std::pair<int,int>> Board::winnerPiece() {
	Array<std::pair<int, int>> res;
	int win = winner();
	if (win == -1) return res;
	else if (win == Piece::WHITE) {

	}
	else if (win == 0) {

	}
}

bool Board::put(const Piece& piece, int cell_number) {
	if (cells[cell_number].add(piece)) {
		
		secondBestTuple = std::make_tuple(INVALID_PIECE, -1, -1);
		piece_num++;
		history.add(piece, cell_number, Direction::NONE, isSecondBestFlag[0], isSecondBestFlag[1]);
		isSecondBestFlag[piece.Owner()] = false;
		turn++;

		return true;
	}
	else {
		// Print << U"don't put: piece:" << piece.Owner() << U" cell:" << cell_number;
		return false;
	}
}

bool Board::move(const Piece& piece, int cell_number, int direction) {
	if (cells[cell_number].top().Owner() != piece.Owner()) return false;
	if (cells[Direction::calc(cell_number, direction)].add(piece)) {
		
		secondBestTuple = std::make_tuple(INVALID_PIECE, -1, -1);
		cells[cell_number].poll();
		history.add(piece, cell_number, direction, isSecondBestFlag[0], isSecondBestFlag[1]);
		isSecondBestFlag[piece.Owner()] = false;
		turn++;

		return true;
	}
	else {
		// Print << U"don't move: piece:" << piece.Owner() << U" cell:" << cell_number << U" direction:" << direction;
		return false;
	}
}

bool Board::back() {
	if (turn == 0) return false;
	std::tuple<Piece, int, int, bool, bool> tuple = history.poll();

	if (std::get<2>(tuple) == Direction::NONE) {
		cells[std::get<1>(tuple)].poll();
		piece_num--;

	}
	else if (std::get<2>(tuple) == Direction::SECONDBEST){
		std::tuple<Piece, int, int, bool, bool> before_tuple = history.top();
		if (std::get<2>(before_tuple) == Direction::NONE) {
			put(std::get<0>(before_tuple), std::get<1>(before_tuple));
			history.poll();
			turn--;
		}
		else {
			int cl = Direction::calc(std::get<1>(before_tuple), std::get<2>(before_tuple));
			move(std::get<0>(before_tuple), cl, Direction::calcDirection(cl, std::get<1>(before_tuple)));
			// Print << U"piece:" << std::get<0>(before_tuple).Owner() << U" cell:" << cl << U" direction:" << Direction::calcDirection(cl, std::get<1>(before_tuple));
			history.poll();
			turn--;
		}
	}
	else {
		Piece piece = cells[Direction::calc(std::get<1>(tuple), std::get<2>(tuple))].poll();
		cells[std::get<1>(tuple)].add(piece);
	}

	turn--;
	isSecondBestFlag[0] = std::get<3>(tuple);
	isSecondBestFlag[1] = std::get<4>(tuple);
	if (turn > 1) {
		std::tuple<Piece, int, int, bool, bool> before_tuple = history.poll();
		if (std::get<2>(before_tuple) == Direction::SECONDBEST) {
			std::tuple<Piece, int, int, bool, bool> before_tuple2 = history.top();
			secondBestTuple = std::make_tuple(std::get<0>(before_tuple2), std::get<1>(before_tuple2), std::get<2>(before_tuple2));
		}
		else {
			secondBestTuple = std::make_tuple(INVALID_PIECE, -1, -1);
		}
		history.add(std::get<0>(before_tuple), std::get<1>(before_tuple), std::get<2>(before_tuple), std::get<3>(before_tuple), std::get<4>(before_tuple));
	}
	
	return true;
}

bool Board::secondBest() {
	std::tuple<Piece, int, int, bool, bool> tuple = history.top();
	if (std::get<2>(tuple) == Direction::SECONDBEST) {
		// Print << U"don't secondBest1";
		return false;
	}
	if (!canSecondBest()) {
		// Print << U"don't secondBest2";
		return false;
	}

	if (std::get<2>(tuple) == Direction::NONE) {
		cells[std::get<1>(tuple)].poll();
		piece_num--;
	}
	else {
		Piece piece = cells[Direction::calc(std::get<1>(tuple), std::get<2>(tuple))].poll();
		cells[std::get<1>(tuple)].add(piece);
	}

	Piece piece(1 - std::get<0>(tuple).Owner());
	history.add(piece, -1, Direction::SECONDBEST, isSecondBestFlag [0], isSecondBestFlag[1]);
	secondBestTuple = std::make_tuple(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
	isSecondBestFlag[piece.Owner()] = true;
	turn++;
	return true;
}

int Board::movePos(int cell_number, int move[]) {
	int res = 0;
	if (cells[cell_number].top().Owner() == Piece::INVALID) return res;

	int left  = Direction::left(cell_number);
	int right = Direction::right(cell_number);
	int front = Direction::front(cell_number);

	if (cells[left].canPut() && !isSecondBestPos(cell_number, Direction::LEFT)) {
		move[res] = left;
		res++;
	}
	if (cells[right].canPut() && !isSecondBestPos(cell_number, Direction::RIGHT)) {
		move[res] = right;
		res++;
	}
	if (cells[front].canPut() && !isSecondBestPos(cell_number, Direction::FRONT)) {
		move[res] = front;
		res++;
	}

	return res;
}

bool Board::canMove(int cell_number) {
	int move[3];
	return movePos(cell_number, move);
}

bool Board::canPut(int cell_number) {
	return cells[cell_number].canPut();
}

bool Board::canSecondBest() {
	return !(isSecondBestFlag[0] || isSecondBestFlag[1] || turn == 0);
}

bool Board::isSecondBest(int index) {
	return isSecondBestFlag[index];
}

bool Board::isSecondBestPos(int cell_number, int direction) {
	return std::get<0>(secondBestTuple).Owner() == (turn & 1) && std::get<1>(secondBestTuple) == cell_number && std::get<2>(secondBestTuple) == direction;
}

bool Board::isPutTime() {
	return piece_num < PIECENUM * 2;
}

Array<int> Board::putLegal() {
	Array<int> res;
	if (isFinish(getNowPiece())) return res;
	for (int i = 0; i < BOARDSIZE; ++i) {
		if (!cells[i].canPut()) continue;
		if (std::get<0>(secondBestTuple).Owner() == (turn & 1) && std::get<1>(secondBestTuple) == i) continue;

		res << i;
	}

	return res;
}

Array<int> Board::moveLegal(int index) {
	Array<int> res;
	if (isFinish(getNowPiece())) return res;
	if (cells[index].top().Owner() != getNowPiece().Owner()) return res;
	int move[3];
	int s = movePos(index, move);
	for (int i = 0; i < s; ++i) {
		res << move[i];
	}

	return res;
}



Cell Board::get(int index) {
	return cells[index];
}

int Board::getPieceNum() {
	return piece_num;
}

int Board::getTurn() {
	return turn;
}

std::tuple<Piece, int, int> Board::getSecondBestTuple() {
	return secondBestTuple;
}

Piece Board::getNowPiece() {
	Piece piece(turn & 1);
	return piece;
}

Piece Board::getOtherPiece() {
	Piece piece(1 - (turn & 1));
	return piece;
}

#include "Player.h"
#include <time.h>


Player::Player() {
	npc_num = 0;
	lock = false;
	start_time = 0;
}

Player::Player(int _npc_num) {
	npc_num = _npc_num;
	lock = false;
	start_time = 0;
}

// 抽象クラス
bool Player::isManual() {
	return npc_num == 0;
}


std::pair<int, int> Player::think(Board board, int time_limit)  {
	switch (npc_num) {
	case RANDOM:
		return random(board, time_limit);
	case GREEDY:
		return greedy(board, time_limit);
	case ALPHABETA:
		return alphaBetaSearch(board, time_limit);
	case MONTECARLO:
		return monteCarlo(board, time_limit);
	default:
		return std::make_pair(-1, -1);
	}
}

void Player::setNPC(int _npc_num) {
	npc_num = _npc_num;
}

// ランダムに手を打つ
std::pair<int, int> Player::random(Board board, int time_limit) {
	int start = clock();
	std::pair<int, int> res = std::make_pair(-1, -1);

	if (board.isPutTime()) {
		Array<int> putLegal = board.putLegal();
		int amax = putLegal.size();
		if (amax > 0) {
			int r = Random(amax - 1);
			res = std::make_pair(putLegal[r], Direction::NONE);
		}
	}
	else {
		Array<std::pair<int, int>> allMoveLegal;
		for (int i = 0; i < Board::BOARDSIZE; ++i) {
			Array<int> moveLegal = board.moveLegal(i);
			for (int j : moveLegal) {
				allMoveLegal << std::make_pair(i, Direction::calcDirection(i, j));
			}
		}
		int amax = allMoveLegal.size();
		if (amax > 0) {
			int r = Random(amax - 1);
			res = allMoveLegal[r];
		}

	}

	while (clock() - start < time_limit);

	return res;
}

// 1手先だけ見て打つ
// 表面の駒の差がなるべく大きくなるように打つ
std::pair<int, int> Player::greedy(Board board, int time_limit) {
	int start = clock();
	std::pair<int, int> res = std::make_pair(-1, -1);
	int max = -Board::BOARDSIZE;

	if (board.isPutTime()) {
		Array<int> legal = board.putLegal();
		Piece piece = board.getNowPiece();
		for (int i : legal) {
			board.put(piece, i);
			if (board.winner() == piece.Owner()) {
				return std::make_pair(i, Direction::NONE);
			}
			int sum = 0;
			for (int j = 0; j < Board::BOARDSIZE; ++j) {
				if (board.get(j).top().Owner() == Piece::INVALID) continue;
				if (board.get(j).top().Owner() == piece.Owner()) ++sum;
				else --sum;
			}
			board.back();

			if (sum > max) {
				max = sum;
				res = std::make_pair(i, Direction::NONE);
			}
		}
	}
	else {
		Array<std::pair<int, int>> allMoveLegal;
		for (int i = 0; i < Board::BOARDSIZE; ++i) {
			Array<int> moveLegal = board.moveLegal(i);
			for (int j : moveLegal) {
				allMoveLegal << std::make_pair(i, Direction::calcDirection(i, j));
			}
		}

		Piece piece = board.getNowPiece();
		for (std::pair<int, int> pr : allMoveLegal) {
			board.move(piece, pr.first, pr.second);
			if (board.winner() == piece.Owner()) {
				return pr;
			}

			int sum = 0;
			for (int j = 0; j < Board::BOARDSIZE; ++j) {
				if (board.get(j).top().Owner() == Piece::INVALID) continue;
				if (board.get(j).top().Owner() == piece.Owner()) ++sum;
				else --sum;
			}
			board.back();

			if (sum > max) {
				max = sum;
				res = pr;
			}
		}
	}

	while (clock() - start < time_limit);

	return res;
}

std::pair<int, int> Player::alphaBetaSearch(Board board, int time_limit) {
	int start = clock();
	std::pair<int, int> res = std::make_pair(-1, -1);
	if (board.getTurn() == 0) {
		res = std::make_pair(board.putLegal()[0], Direction::NONE);
		while (clock() - start > time_limit);
		return res;
	}

	int deep = 3;
	int a = -EVALUATEMAX-1;
	int b = EVALUATEMAX;
	Piece piece = board.getNowPiece();
	Board bd(board);
	while (clock() - start < time_limit) {
		int eval;

		// SecondBest
		if (board.canSecondBest()) {
			board.secondBest();
			eval = -negaAlpha(board, deep+1, -b, -a, 1-piece.Owner());
			// board.back();
			board = bd;

			if (a < eval) {
				a = eval;
				res = std::make_pair(-1, Direction::SECONDBEST);
			}
			if (a >= b) {
				return std::make_pair(-1, Direction::SECONDBEST);
			}
		}
		// Put
		if (board.isPutTime()) {
			Array<int> legal = board.putLegal();
			for (int i : legal) {
				board.put(piece, i);
				eval = -negaAlpha(board, deep - 1, -b, -a, 1-piece.Owner());
				board.back();

				if (a < eval) {
					a = eval;
					res = std::make_pair(i, Direction::NONE);
				}
				if (a >= b) {
					return std::make_pair(i, Direction::NONE);
				}
			}
		}
		// Move
		else {
			for (int i = 0; i < Board::BOARDSIZE; ++i) {
				Array<int> legal = board.moveLegal(i);
				for (int j : legal) {
					board.move(piece, i, Direction::calcDirection(i, j));
					eval = -negaAlpha(board, deep - 1, -b, -a, 1-piece.Owner());
					board.back();

					if (a < eval) {
						a = eval;
						res = std::make_pair(i, Direction::calcDirection(i, j));
					}
					if (a >= b) {
						return std::make_pair(i, Direction::calcDirection(i, j));
					}
				}
			}
		}

		// Print << U"deep:" << deep;
		++deep;
	}

	// Print << U"piece:" << piece.Owner() << U" cell:" << res.first << U" direction:" << res.second;
	return res;

}

// 未完成
std::pair<int, int> Player::monteCarlo(Board board, int time_limit) {
	return std::make_pair(-1, -1);
}


int Player::negaAlpha(Board& board, int deep, int a, int b, int t) {
	if (board.winner() != -1) return endEvaluate(board, t);
	if (deep <= 0) return evaluate(board, t);
	int eval;
	Piece piece = board.getNowPiece();

	Board bd(board);
	// SecondBest
	if (board.canSecondBest()) {
		board.secondBest();
		eval = -negaAlpha(board, deep+1, -b, -a, 1-t);
		// board.back();
		board = bd;

		if (a < eval) {
			a = eval;
		}
		if (a >= b) {
			return a;
		}
	}
	// Put
	if (board.isPutTime()) {
		Array<int> legal = board.putLegal();
		for (int i : legal) {
			board.put(piece, i);
			eval = -negaAlpha(board, deep-1, -b, -a, 1-t);
			board.back();

			if (a < eval) {
				a = eval;
			}
			if (a >= b) {
				return a;
			}
		}
	}
	// Move
	else {
		for (int i = 0; i < Board::BOARDSIZE; ++i) {
			Array<int> legal = board.moveLegal(i);
			for (int j : legal) {
				board.move(piece, i, Direction::calcDirection(i, j));
				eval = -negaAlpha(board, deep-1, -b, -a, 1-t);
				board.back();

				if (a < eval) {
					a = eval;
				}
				if (a >= b) {
					return a;
				}
			}
		}
	}

	return a;
}

int Player::endEvaluate(Board& board, int t) {
	int win = board.winner();
	if (win == t) return EVALUATEMAX - board.getTurn();
	// else if (win == 1-t) return -EVALUATEMAX;
	else return -EVALUATEMAX + board.getTurn();
}


int Player::evaluate(Board& board, int t) {
	int value = 0;
	Piece piece(t);
	Piece piece_other(1 - t);
	if (board.isFinish(piece)) value += 50;
	if (board.isFinish(piece_other)) value -= 50;

	// 可動範囲が多いほど良い
	int move[3];
	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		Piece p = board.get(i).top();
		if (p.Owner() == Piece::INVALID) continue;
		if (p.Owner() == piece.Owner()) {
			// value += 50;
			value += board.movePos(i, move);
		}
		else {
			// value -= 50;
			value -= board.movePos(i, move);
		}
	}



	// chain数が多いほど良い
	int eval_chain[] = {0, 0, 2, 8, 0};
	value += eval_chain[board.chainNum(piece)];
	value -= eval_chain[board.chainNum(piece_other)];

	// 積み重なっている程良い
	int eval_stack[] = {0, 0, 10, 0};
	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		value += eval_stack[board.stackNum(piece, i)];
		value -= eval_stack[board.stackNum(piece_other, i)];
	}

	return value;
}

#pragma once
#include "Cell.h"
#include "History.h"

class Board {
public:
	static const int BOARDSIZE = 8;
	static const int PIECENUM = 8;

private:
	Piece INVALID_PIECE;
	Cell cells[BOARDSIZE];
	int turn;
	int piece_num;
	bool isSecondBestFlag[2];
	std::tuple<Piece, int, int> secondBestTuple;
	History history;

public:
	Board();
	Board(Board& board);

	bool isChain(const Piece& piece);
	bool isStackUp(const Piece& piece);
	bool isImmovable(const Piece& piece);
	bool isFinish(const Piece& piece);
	bool isFinish();
	int winner();
	Array<std::pair<int,int>> winnerPiece();

	int chainNum(const Piece& piece);
	int stackNum(const Piece& piece, int index);

	bool put(const Piece& piece, int cell_number);
	bool move(const Piece& piece, int cell_number, int direction);
	bool back();
	bool secondBest();

	int movePos(int cell_number, int move[]);
	bool canMove(int cell_number);
	bool canPut(int cell_number);
	bool canSecondBest();
	bool isSecondBest(int index);
	bool isSecondBestPos(int cell_number, int direction);
	bool isPutTime();

	Array<int> putLegal();
	Array<int> moveLegal(int index);

	Cell get(int index);
	int getPieceNum();
	int getTurn();
	std::tuple<Piece, int, int> getSecondBestTuple();
	Piece getNowPiece();
	Piece getOtherPiece();

};



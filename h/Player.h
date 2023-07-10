#pragma once
#include "Board.h"
#include "Direction.h"
// 抽象クラス
class Player {
public:
	static const int MANUAL     = 0;
	static const int RANDOM     = 1;
	static const int GREEDY     = 2;
	static const int ALPHABETA  = 3;
	static const int MONTECARLO = 4;
	static const int EVALUATEMAX = 10000;

private:
	int npc_num;
	bool lock;
	int start_time;
	AsyncTask<std::pair<int, int>> task;
	

public:
	Player();
	Player(int _npc_num);
	void setNPC(int _npc_num);
	std::pair<int, int> think(Board board, int time_limit);
	bool isManual();

private:
	static std::pair<int, int> random(Board board, int time_limit);
	static std::pair<int, int> greedy(Board board, int time_limit);
	static std::pair<int, int> alphaBetaSearch(Board board, int time_limit);
	static std::pair<int, int> monteCarlo(Board board, int time_limit);

	static int negaAlpha(Board& board, int deep, int a, int b, int t);
	static int endEvaluate(Board& board, int t);
	static int evaluate(Board& board, int t);
};

#include "Game.h"
#include "Direction.h"

Item::Item(const Vec2& _center, const ColorF& _color) {
	center = _center;
	color = _color;
}

void Item::draw() const {
	Circle{ center, circleR }.drawFrame(1, 1, color);
}

ItemPiece::ItemPiece() {
	frameSize = 1.0;
}

ItemPiece::ItemPiece(const Circle& _circle) {
	circle = _circle;
	color = Palette::White;
	colorFrame = Palette::Black;
	frameSize = 1.0;
}

ItemPiece::ItemPiece(const Circle& _circle, const ColorF& _color) {
	circle = _circle;
	color = _color;
	colorFrame = Palette::Black;
	frameSize = 1.0;
}

void ItemPiece::draw() const {
	circle.draw(color).drawFrame(frameSize, frameSize, colorFrame);
}

void ItemPiece::set(const Circle& _circle, const ColorF& _color, const ColorF& _colorFrame) {
	circle = _circle;
	color = _color;
	colorFrame = _colorFrame;
}

void ItemPiece::setColorFrame(const ColorF& _colorFrame) {
	colorFrame = _colorFrame;
}

ColorF ItemPiece::getColorFrame() {
	return colorFrame;
}

void ItemPiece::setFrameSize(const int _frameSize) {
	frameSize = _frameSize;
}

int ItemPiece::getFrameSize() {
	return frameSize;
}

bool ItemPiece::mouseOver() {
	return circle.mouseOver();
}

bool ItemPiece::leftClicked() {
	return circle.leftClicked();
}


SecondBestButton::SecondBestButton() {
	Circle _circle(Scene::Center(), circleR);
	circle = _circle;
	lock = false;
}

void SecondBestButton::setLock(bool _lock) {
	lock = _lock;
}

void SecondBestButton::draw() const {
	if (lock) {
		circle.draw(Palette::Gray);
		emoji.scaled(0.75).drawAt(Scene::Center(), Palette::Gray);
	}
	else if (circle.mouseOver()) {
		circle.draw(Palette::Cyan);
		emoji.scaled(0.75).drawAt(Scene::Center(), Palette::Cyan);
	}
	else {
		circle.draw(Palette::White);
		emoji.scaled(0.75).drawAt(Scene::Center());
	}
}

bool SecondBestButton::leftClicked() {
	if (lock) return false;

	return circle.leftClicked();
}


Game::Game() {
	x = Scene::Center().x;
	y = Scene::Center().y;
	clickPos = -1;
	lock = true;

	white_player.setNPC(0);
	gray_player.setNPC(1);

	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		double t = 2.0 * Math::Pi * i / Board::BOARDSIZE;
		Vec2 vec(x + circlesR*Math::Cos(t), y + circlesR * Math::Sin(t));
		Item item(vec, Palette::Yellow);
		items << item;
	}
}

void Game::init() {
	Board b;
	board = b;
	
	clickPos = -1;
}

void Game::setPlayer(int white_npc, int gray_npc) {
	white_player.setNPC(white_npc);
	gray_player.setNPC(gray_npc);
}

void Game::draw() {
	setItemPiece();
	secondBestButton.setLock(!board.canSecondBest());
	secondBestButton.draw();
	
	

	// ボードを描画する
	for (const auto& item : items) {
		item.draw();
	}

	int winner = board.winner();
	// int winner = -1;
	// 勝敗が決まっていないとき
	if (winner == -1) {
		// 入力があった場合、設置や移動等を行う
		play();
		if (board.getNowPiece().Owner() == Piece::WHITE) {
			if (white_player.isManual()) {
				// マウスが入っている場所の色を変える
				setDrawMouseOver();

				// 左クリックした場所の色を変える
				setLeftClicked();
				setDrawLeftClicked();
			}
		}
		else {
			if (gray_player.isManual()) {
				// マウスが入っている場所の色を変える
				setDrawMouseOver();

				// 左クリックした場所の色を変える
				setLeftClicked();
				setDrawLeftClicked();
			}
		}

		// secondBestされた場所の色を変える
		setDrawSecondBestPos();

		// 駒を描画する
		drawItemPiece();
	}
	// 白が勝ったとき、
	else if (winner == Piece::WHITE) {
		// 駒を描画する
		drawItemPiece();

		font(U"WHITE WIN!").drawAt(Scene::Center(), Palette::Gold);
	}
	// 灰が勝ったとき、
	else if (winner == Piece::GRAY) {
		// 駒を描画する
		drawItemPiece();

		font(U"GRAY WIN!").drawAt(Scene::Center(), Palette::Gold);
	}
	// 引き分けのとき、
	else {
		// 駒を描画する
		drawItemPiece();

		font(U"DRAW").drawAt(Scene::Center(), Palette::Gold);
	}

	

	

}

void Game::back() {
	board.back();
}

void Game::drawItemPiece() {
	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		Cell cell = board.get(i);

		for (int j = 0; j < Cell::HEIGHT_MAX; ++j) {
			Piece piece = cell.get(j);
			if (piece.Owner() == Piece::INVALID && j > 0) break;

			if (item_pieces[i][j].getColorFrame() != (ColorF)Palette::Black) {
				item_pieces[i][j].setFrameSize(2);
			}
			item_pieces[i][j].draw();
		}
	}
}

void Game::setDrawMouseOver() {
	if (board.isPutTime()) {
		Array<int> legal = board.putLegal();
		for (int i : legal) {
			Cell cell = board.get(i);
			if (!cell.canPut()) continue;

			if (cell.size() == 0) {
				if (item_pieces[i][cell.size()].mouseOver()) {
					item_pieces[i][cell.size()].setColorFrame(Palette::Cyan);
				}
			}
			else {
				if (item_pieces[i][cell.size()-1].mouseOver()) {
					item_pieces[i][cell.size()-1].setColorFrame(Palette::Cyan);
				}
			}
			
			
		}
	}
	else {
		if (board.isFinish()) return;
		if (clickPos == -1) {
			for (int i = 0; i < Board::BOARDSIZE; ++i) {
				Cell cell = board.get(i);
				if (cell.top().Owner() != (board.getTurn() & 1)) continue;

				if (item_pieces[i][cell.size() - 1].mouseOver()) {
					item_pieces[i][cell.size() - 1].setColorFrame(Palette::Cyan);
				}
			}
		}
		else {
			int left = Direction::left(clickPos);
			int right = Direction::right(clickPos);
			int front = Direction::front(clickPos);

			if (board.get(left).canPut()) {
				int top_num = board.get(left).size();
				if (item_pieces[left][top_num].mouseOver()) {
					item_pieces[left][top_num].setColorFrame(Palette::Cyan);
				}
			}
			if (board.get(right).canPut()) {
				int top_num = board.get(right).size();
				if (item_pieces[right][top_num].mouseOver()) {
					item_pieces[right][top_num].setColorFrame(Palette::Cyan);
				}
			}
			if (board.get(front).canPut()) {
				int top_num = board.get(front).size();
				if (item_pieces[front][top_num].mouseOver()) {
					item_pieces[front][top_num].setColorFrame(Palette::Cyan);
				}
			}
		}
	}
}

void Game::setDrawLeftClicked() {
	if (clickPos != -1) {
		item_pieces[clickPos][board.get(clickPos).size() - 1].setColorFrame(Palette::Blue);

		Array<int> moveLegal = board.moveLegal(clickPos);
		for (int ml : moveLegal) {
			Cell cell = board.get(ml);
			if (!cell.canPut()) continue;

			if (cell.size() == 0) {
				item_pieces[ml][cell.size()].setColorFrame(Palette::Cyan);
			}
			else {
				item_pieces[ml][cell.size() - 1].setColorFrame(Palette::Cyan);
			}
		}

	}
}

void Game::setLeftClicked() {
	if (MouseL.down()) {
		clickPos = -1;
	}

	if (!board.isPutTime() && !board.isFinish()) {
		for (int i = 0; i < Board::BOARDSIZE; ++i) {
			Cell cell = board.get(i);
			if (cell.top().Owner() != (board.getTurn() & 1)) continue;

			if (item_pieces[i][cell.size() - 1].leftClicked()) {
				clickPos = i;
				break;
			}
		}
	}
}

void Game::setDrawSecondBestPos() {
	std::tuple<Piece, int, int> tuple = board.getSecondBestTuple();
	if (std::get<0>(tuple).Owner() == (board.getTurn() & 1)) {
		if (board.isPutTime()) {
			Cell cell = board.get(std::get<1>(tuple));
			if (cell.size() == 0) {
				item_pieces[std::get<1>(tuple)][cell.size()].setColorFrame(Palette::Red);
			}
			else {
				item_pieces[std::get<1>(tuple)][cell.size() - 1].setColorFrame(Palette::Red);
			}
		}
		else {
			if (clickPos != std::get<1>(tuple)) return;
			int m = Direction::calc(std::get<1>(tuple), std::get<2>(tuple));
			Cell cell = board.get(m);
			
			if (cell.size() == 0) {
				item_pieces[m][cell.size()].setColorFrame(Palette::Red);
			}
			else {
				item_pieces[m][cell.size() - 1].setColorFrame(Palette::Red);
			}
		}
	}
}


void Game::setDrawLegal() {
	// if () {

	// }
}


void Game::setItemPiece() {
	for (int i = 0; i < Board::BOARDSIZE; ++i) {
		Cell cell = board.get(i);
		double t = 2.0 * Math::Pi * i / Board::BOARDSIZE;
		Vec2 vec(x + circlesR * Math::Cos(t), y + circlesR * Math::Sin(t));

		for (int j = 0; j < Cell::HEIGHT_MAX; ++j) {
			Piece piece = cell.get(j);
			ItemPiece item_piece;
			Circle circle(vec, circleR);
			if (piece.Owner() == Piece::INVALID) {
				item_piece.set(circle, Palette::Black, Palette::Black);
			}
			else if (piece.Owner() == Piece::WHITE) {
				item_piece.set(circle, Palette::White, Palette::Black);
			}
			else {
				item_piece.set(circle, Palette::Gray, Palette::Black);
			}

			item_pieces[i][j] = item_piece;
			vec.y -= circleR / 5;
		}
	}
}



void Game::play() {
	if (board.getNowPiece().Owner() == Piece::WHITE) {
		if (white_player.isManual()) {
			manualPlay();
		}
		else {
			autoPlay(white_player);
		}
	}
	else {
		if (gray_player.isManual()) {
			manualPlay();
		}
		else {
			autoPlay(gray_player);
		}
	}
}


//
void Game::manualPlay() {
	// SecondBest
	if (secondBestButton.leftClicked()) {
		board.secondBest();
	}
	// 設置
	if (board.isPutTime()) {
		Array<int> putLegal = board.putLegal();
		for (int i : putLegal) {
			Cell cell = board.get(i);
			if (!cell.canPut()) continue;

			if (cell.size() == 0) {
				if (item_pieces[i][cell.size()].leftClicked()) {
					board.put(board.getNowPiece(), i);
					break;
				}
			}
			else {
				if (item_pieces[i][cell.size() - 1].leftClicked()) {
					board.put(board.getNowPiece(), i);
					break;
				}
			}


		}
	}
	// 移動
	else {
		if (clickPos != -1) {
			Array<int> moveLegal = board.moveLegal(clickPos);
			for (int i : moveLegal) {
				Cell cell = board.get(i);
				if (!cell.canPut()) continue;

				if (cell.size() == 0) {
					if (item_pieces[i][cell.size()].leftClicked()) {
						board.move(board.getNowPiece(), clickPos, Direction::calcDirection(clickPos, i));
						clickPos = -1;
						break;
					}
				}
				else {
					if (item_pieces[i][cell.size() - 1].leftClicked()) {
						board.move(board.getNowPiece(), clickPos, Direction::calcDirection(clickPos, i));
						clickPos = -1;
						break;
					}
				}
			}
		}
	}
}


void Game::autoPlay(Player &player) {
	if (lock) {
		lock = false;
		return;
	}
	std::pair<int, int> move = player.think(board, 150);

	if (move.second == Direction::SECONDBEST) {
		board.secondBest();
	}
	else if (move.second == Direction::NONE) {
		board.put(board.getNowPiece(), move.first);
	}
	else {
		board.move(board.getNowPiece(), move.first, move.second);
	}

	lock = true;
}


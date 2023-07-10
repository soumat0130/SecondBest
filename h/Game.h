#pragma once
#include "Board.h"
#include "Player.h"
#include <Siv3D.hpp>

struct Item {
public:
	static constexpr double circleR = 60.0;

private:
	
	Vec2 center;
	ColorF color;

public:
	Item(const Vec2& _center, const ColorF& _color);

	void draw() const;
};

struct ItemPiece {
public:
	static constexpr double circleR = 60.0;

private:

	Circle circle;
	ColorF color;
	ColorF colorFrame;
	double frameSize;

public:
	ItemPiece();
	ItemPiece(const Circle& _circle);
	ItemPiece(const Circle& _circle, const ColorF& _color);

	void draw() const;
	void set(const Circle& _circle, const ColorF& _color, const ColorF& _colorFrame);
	void setColorFrame(const ColorF& _colorFrame);
	ColorF getColorFrame();
	void setFrameSize(const int _frameSize);
	int getFrameSize();
	bool mouseOver();
	bool leftClicked();
};


struct SecondBestButton {
public:
	static constexpr double circleR = 24.0;

private:
	Circle circle;
	bool lock;
	const Texture emoji{ U"🫵"_emoji };

public:
	SecondBestButton();

	void setLock(bool _lock);
	void draw() const;
	bool leftClicked();


};



class Game {
private:
	Board board;
	Array<Item> items;
	int clickPos;
	ItemPiece item_pieces[Board::BOARDSIZE][Cell::HEIGHT_MAX];
	SecondBestButton secondBestButton;
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
	bool lock;

	int x, y;
	const double circleR = 54.0;
	const double circlesR = 200.0;

	Player white_player;
	Player gray_player;

public:
	Game();
	void init();
	void setPlayer(int white_npc, int gray_npc);
	void draw();
	void back();

private:
	void setItemPiece();
	void drawItemPiece();
	void setDrawMouseOver();
	void setDrawLeftClicked();
	void setLeftClicked();
	void setDrawSecondBestPos();
	void setDrawLegal();
	void play();
	void manualPlay();
	void autoPlay(Player& player);
	

};

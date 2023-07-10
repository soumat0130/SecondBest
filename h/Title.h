#pragma once

class Title {
private:
	const Font font{ FontMethod::MSDF, 24, Typeface::Bold };
	const Array<String> items = {U"You", U"NPC1", U"NPC2", U"NPC3"};
	size_t index_white;
	size_t index_gray;

public:
	Title();

	void draw();
	int getIndexWhite();
	int getIndexGray();
	bool startButton();
	bool endButton();
};



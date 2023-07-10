#include "Title.h"
#include <Siv3D.hpp>

Title::Title() {
	index_white = 0;
	index_gray  = 1;
}

void Title::draw() {
	font(U"SECOND BEST!").drawAt(60, Vec2{Scene::Center().x, Scene::Center().y - 200}, Palette::Gold);
	int dy = 50;
	SimpleGUI::RadioButtonsAt(index_white, items, Vec2{Scene::Center().x - 100, Scene::Center().y + dy });
	SimpleGUI::RadioButtonsAt(index_gray, items, Vec2{ Scene::Center().x + 100, Scene::Center().y + dy });
	font(U"VS").drawAt(48, Vec2{ Scene::Center().x, Scene::Center().y + dy }, Palette::Red);
}

int Title::getIndexWhite() {
	return index_white;
}

int Title::getIndexGray() {
	return index_gray;
}

bool Title::startButton() {
	return SimpleGUI::ButtonAt(U"Start!", Vec2{ Scene::Center().x, Scene::Center().y - 100 }, 300);
}

bool Title::endButton() {
	return SimpleGUI::ButtonAt(U"Exit", Vec2{Scene::Center().x, Scene::Center().y + 200}, 300);
}

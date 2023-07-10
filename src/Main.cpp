# include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include "Game.h"
#include "Title.h"

void Main()
{
	
	// 背景の色を設定する | Set the background color
	// Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
	Scene::SetBackground(Palette::Black);

	// 画像ファイルからテクスチャを作成する | Create a texture from an image file
	const Texture texture{ U"example/windmill.png" };

	// 絵文字からテクスチャを作成する | Create a texture from an emoji
	const Texture emoji{ U"🦖"_emoji };

	// 太文字のフォントを作成する | Create a bold font with MSDF method
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// テキストに含まれる絵文字のためのフォントを作成し、font に追加する | Create a font for emojis in text and add it to font as a fallback
	const Font emojiFont{ 48, Typeface::ColorEmoji };
	font.addFallback(emojiFont);

	// ボタンを押した回数 | Number of button presses
	int32 count = 0;

	// チェックボックスの状態 | Checkbox state
	bool checked = false;

	// プレイヤーの移動スピード | Player's movement speed
	double speed = 200.0;

	// プレイヤーの X 座標 | Player's X position
	double playerPosX = 400;

	// プレイヤーが右を向いているか | Whether player is facing right
	bool isPlayerFacingRight = true;

	int scene = 0;

	Title title;

	Game game;

	const Array<std::pair<String, Array<String>>> menus{
		{U"ゲーム",{U"一手戻す", U"タイトルへ戻る", U"終了"}}
	};

	SimpleMenuBar menuBar{ menus };
	

	while (System::Update())
	{
		if (scene == 0) {
			title.draw();

			if (title.startButton()) {
				game.init();
				game.setPlayer(title.getIndexWhite(), title.getIndexGray());
				scene = 1;
			}
			if (title.endButton()) {
				return;
			}
		}
		else {
			if (const auto item = menuBar.update()) {
				if (item == MenuBarItemIndex{ 0, 2 }) {
					return;
				}

				if (item == MenuBarItemIndex{ 0, 1 }) {
					scene = 0;
				}

				if (item == MenuBarItemIndex{ 0, 0 }) {
					game.back();
				}
			}

			menuBar.draw();
			game.draw();
		}
	}
	
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//

#include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{

}

void Title::update()
{
	if (Key1.down())
	{
		getData().boardSize = {9, 9};
		getData().mineCount = 10;
		getData().startGridPos = {0, 0};
		getData().keyGridPos = {4, 4};
		getData().goalGridPos = {8, 8};
		changeScene(U"Game");
	}
	else if (Key2.down())
	{
		getData().boardSize = {16, 16};
		getData().mineCount = 40;
		getData().startGridPos = {0, 5};
		getData().keyGridPos = {7, 10};
		getData().goalGridPos = {15, 8};

		changeScene(U"Game");
	}
	else if (Key3.down())
	{
		getData().boardSize = {24, 16};
		getData().mineCount = 80;
		getData().startGridPos = {0, 3};
		getData().keyGridPos = {7, 12};
		getData().goalGridPos = {23, 9};
		changeScene(U"Game");
	}
}

void Title::draw() const
{
	Scene::SetBackground(Palette::Black);

	FontAsset(U"Number")(U"MINESWEEPER MAZE").drawAt(Scene::Center().x, 250, Palette::White);

	double alpha = Periodic::Sine0_1(5.0s);
	FontAsset(U"Message")(U"Press [1] for Easy").drawAt(Scene::Center().x, 500, ColorF{0.5, 1.0, 0.5, alpha});
    FontAsset(U"Message")(U"Press [2] for Normal").drawAt(Scene::Center().x, 600, ColorF{1.0, 1.0, 0.5, alpha});
    FontAsset(U"Message")(U"Press [3] for Hard").drawAt(Scene::Center().x, 700, ColorF{1.0, 0.5, 0.5, alpha});
}

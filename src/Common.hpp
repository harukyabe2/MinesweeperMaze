#pragma once
#include <Siv3D.hpp>

struct GameData
{
	Size boardSize = {0, 0};
	int32 mineCount = 0;
	Point startGridPos = {0, 0};
	Point keyGridPos = {0, 0};
	Point goalGridPos = {0, 0};
};

using App = SceneManager<String, GameData>;

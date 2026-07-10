#include <Siv3D.hpp>
#include "Game.hpp"
#include "Title.hpp"
#include "Common.hpp"

bool Initialize();
void LoadData();

void Main()
{
	if (!Initialize()) return;

	App manager;
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");

	while (System::Update())
	{
		if (not manager.update()) break;
	}
}



bool Initialize()
{
	Scene::Resize(1500, 900);
	Scene::SetResizeMode(ResizeMode::Keep);
    Window::SetStyle(WindowStyle::Sizable);
    Window::Maximize();
    Window::SetTitle(U"Minesweeper Game");

    LoadData();

    return true;
}

void LoadData()
{
    FontAsset::Register(U"Number", FontMethod::MSDF, 48, Typeface::Bold);
    FontAsset::Register(U"Message", FontMethod::MSDF, 48, Typeface::Bold);
    FontAsset::Register(U"Time", FontMethod::MSDF, 24, Typeface::Bold);

    TextureAsset::Register(U"Mine", U"💣"_emoji);
    TextureAsset::Register(U"Flag", U"🚩"_emoji);
	TextureAsset::Register(U"Key", U"🗝️"_emoji);	
	TextureAsset::Register(U"Goal", U"🚪"_emoji);	
	TextureAsset::Register(U"Life", U"❤️"_emoji);	

	TextureAsset::Register(U"Block1", U"imgs/tile_0019.png");
	TextureAsset::Register(U"Block2", U"imgs/tile_0020.png");
	TextureAsset::Register(U"Block3", U"imgs/tile_0027.png");
	TextureAsset::Register(U"Wall", U"imgs/tile_0037.png");
	TextureAsset::Register(U"Human_stand", U"imgs/character_femaleAdventurer_side.png");
	TextureAsset::Register(U"Human_walk1", U"imgs/character_femaleAdventurer_walk0.png");
	TextureAsset::Register(U"Human_walk2", U"imgs/character_femaleAdventurer_walk1.png");
	TextureAsset::Register(U"Human_damage", U"imgs/character_femaleAdventurer_shoveBack.png");
	TextureAsset::Register(U"Human_damage2", U"imgs/character_femaleAdventurer_shoveBack_white.png");

	AudioAsset::Register(U"Key", Resource(U"sounds/GB-Action01-09(Item).mp3"));
	AudioAsset::Register(U"Break", Resource(U"sounds/SNES-RPG01-01(Chest).mp3"));
	AudioAsset::Register(U"Walk", Resource(U"sounds/SNES-RPG01-05(Stairs).mp3"));
	AudioAsset::Register(U"Mine", Resource(U"sounds/SNES-RPG01-03(Door).mp3"));
	AudioAsset::Register(U"Clear", Resource(U"sounds/Arcade-Action01-4(Score).mp3"));
	AudioAsset::Register(U"GameOver", Resource(U"sounds/GB-Action01-06(Miss).mp3"));

	for (const auto& [name, info] : TextureAsset::Enumerate())
    {
        TextureAsset::Load(name);
    }
    
    for (const auto& [name, info] : AudioAsset::Enumerate())
    {
        AudioAsset::Load(name);
    }
}

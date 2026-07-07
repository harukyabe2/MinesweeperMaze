#include "Game.hpp"

Game::Game()
: mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mPlayer(Point{0, 0})
, mLife(3)
, mHasKey(false)
, mLeftClicked(false)
, mRightClicked(false)
, mKeyCIsPressed(false)
, mKeyEIsPressed(false)
, mClickPos(0, 0)
{

}

bool Game::Initialize()
{
	Scene::Resize(1500, 900);
	Scene::SetResizeMode(ResizeMode::Keep);
    Window::SetStyle(WindowStyle::Sizable);
    Window::Maximize();
    Window::SetTitle(U"Minesweeper Game");

    LoadData();

    return true;
}

void Game::RunLoop()
{
    while (System::Update())
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    if (mState == GameState::isPlaying)
    {
		const auto t = mCamera.createTransformer();

		mLeftClicked = MouseL.down();
		if (mLeftClicked) mClickPos = Cursor::PosF();
		
		mRightClicked = MouseR.down();
		if (mRightClicked) mClickPos = Cursor::PosF();
		
    }
	else if (mState != GameState::isPlaying)
	{
		mKeyCIsPressed = KeyC.down();
		mKeyEIsPressed = KeyE.down();
	}
}

void Game::UpdateGame()
{
	if (mLeftClicked || mRightClicked)
	{
		Point targetGridPos = mBoard.GetGridPosFromScreenPos(mClickPos);

		if (mBoard.IsValidGridPos(targetGridPos))
		{
			if (mLeftClicked)
			{
				Point playerGridPos = mPlayer.GetGridPos();

				if (mBoard.CanOpen(targetGridPos, playerGridPos))
				{
					bool hitMine = mBoard.OpenCell(targetGridPos);

					if (hitMine)
					{
						--mLife;
						if (mLife <= 0) mState = GameState::isGameOver;
					}
					else
					{
						Point closestPos = mBoard.GetClosestReachableGridPos(playerGridPos, targetGridPos);
						Array<Point> path = mBoard.FindPathBFS(playerGridPos, closestPos);
						mPlayer.SetPath(path);
					}
				}
				else if (mBoard.IsOpenedGridPos(targetGridPos))
				{
					Array<Point> path = mBoard.FindPathBFS(playerGridPos, targetGridPos);
					if (!path.empty()) mPlayer.SetPath(path);
				}
			}
			else if (mRightClicked)
			{
				mBoard.ToggleFlag(targetGridPos);
			}
		}
	}

	if (mState == GameState::isPlaying)
	{
		mPlayer.Update();

		Point playerGridPos = mPlayer.GetGridPos();

		if (playerGridPos == mBoard.GetKeyGridPos())
		{
			mHasKey = true;
			mBoard.RemoveKey();
		}

		if (playerGridPos == mBoard.GetGoalGridPos() && mHasKey) mState = GameState::isGameClear;
	}

	if (mState != GameState::isPlaying)
	{
		if (mKeyCIsPressed)
		{
			mBoard.Reset();
			mBoard.CreateBoard({24, 16}, 80, Point{0, 3}, Point{7, 12}, Point{23, 9});
			mPlayer = Player(Point{0, 3});
			mState = GameState::isPlaying;
			mLife = 3;
			mHasKey = false;
			mKeyCIsPressed = false;
		}
		else if (mKeyEIsPressed)
		{
			System::Exit();
		}
	}
}

void Game::GenerateOutput()
{
    Scene::SetBackground(Palette::Black);

    const auto t = mCamera.createTransformer();

    mBoard.Draw();

	if (mState == GameState::isPlaying)
	{
		Point hoverGridPos = mBoard.GetGridPosFromScreenPos(Cursor::PosF());
		if (mBoard.IsValidGridPos(hoverGridPos))
		{
			Point hoverScreenPos = mBoard.GetScreenPosFromGridPos(hoverGridPos);
			Rect{ Arg::center(hoverScreenPos), 50, 50 }.draw(ColorF{1.0, 0.3});
		}
	}

	Vec2 playerScreenPos = mBoard.GetScreenPosFromGridPos(mPlayer.GetGridPos());
	mPlayer.Draw(playerScreenPos);

	for (int32 i = 0; i < mLife; ++i) TextureAsset(U"Life").scaled(0.3).draw(630 + i * 40, -390);

	if (mHasKey) TextureAsset(U"Key").scaled(0.3).draw(630, -340);

	if (mState == GameState::isGameOver)
    {
		RectF(Vec2{ -640, -450 }, Size{ 1280, 900 }).draw(ColorF{ 0.0, 0.7 });
        FontAsset(U"Message")(U"GAME OVER").drawAt(100, {0, -150}, Palette::Red);

		double alpha = Periodic::Sine0_1(2.0s);
        FontAsset(U"Message")(U"PRESS [C] to Continue").drawAt({0, 120}, ColorF{1.0, alpha});
        FontAsset(U"Message")(U"PRESS [E] or [Esc] to End").drawAt({0, 190}, ColorF{1.0, alpha});
    }
    else if (mState == GameState::isGameClear)
    {
		RectF(Vec2{ -640, -450 }, Size{ 1280, 900 }).draw(ColorF{ 0.0, 0.7 });
        FontAsset(U"Message")(U"GAME CLEAR").drawAt({0, -150}, Palette::Yellow);

		double alpha = Periodic::Sine0_1(2.0s);
        FontAsset(U"Message")(U"PRESS [C] to Continue").drawAt({0, 120}, ColorF{1.0, alpha});
        FontAsset(U"Message")(U"PRESS [E] or [Esc] to End").drawAt({0, 190}, ColorF{1.0, alpha});
    }
}

void Game::LoadData()
{
    FontAsset::Register(U"Number", FontMethod::MSDF, 48, Typeface::Bold);
    FontAsset::Register(U"Message", FontMethod::MSDF, 48, Typeface::Bold);
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

	mBoard.CreateBoard({24, 16}, 80, Point{0, 3}, Point{7, 12}, Point{23, 9});
	mPlayer = Player(Point{0, 3});
}

void Game::Shutdown()
{
}

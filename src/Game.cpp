#include "Game.hpp"

Game::Game()
: mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mPlayer(Point{0, 0})
, mLeftClicked(false)
, mRightClicked(false)
, mKeyCIsPressed(false)
, mKeyEIsPressed(false)
, mClickPos(0, 0)
{

}

bool Game::Initialize()
{
	Scene::Resize(1280, 900);
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
					mBoard.OpenCell(targetGridPos);

					Point closestPos = mBoard.GetClosestReachableGridPos(playerGridPos, targetGridPos);
					mPlayer.SetGridPos(closestPos);
				}
				else if (mBoard.IsOpenedGridPos(targetGridPos))
				{
					Array<Point> path = mBoard.FindPathBFS(playerGridPos, targetGridPos);
					if (!path.empty()) mPlayer.SetGridPos(targetGridPos);
				}
			}
			else if (mRightClicked)
			{
				mBoard.ToggleFlag(targetGridPos);
			}
		}
	}

    if (mBoard.IsGameOver()) mState = GameState::isGameOver;
    else if (mBoard.IsCleared()) mState = GameState::isGameClear;

	if (mState != GameState::isPlaying)
	{
		if (mKeyCIsPressed)
		{
			mBoard.Reset();
			mBoard.CreateBoard({24, 16}, 80, Point{0, 3}, Point{7, 12}, Point{23, 9});
			mPlayer = Player(Point{0, 3});
			mState = GameState::isPlaying;
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

	Point playerScreenPos = mBoard.GetScreenPosFromGridPos(mPlayer.GetGridPos());
	mPlayer.Draw(playerScreenPos);

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
	TextureAsset::Register(U"Cat", U"🐱"_emoji);	
	TextureAsset::Register(U"Key", U"🗝️"_emoji);	
	TextureAsset::Register(U"Goal", U"🏳️"_emoji);	

	mBoard.CreateBoard({24, 16}, 80, Point{0, 3}, Point{7, 12}, Point{23, 9});
	mPlayer = Player(Point{0, 3});
}

void Game::Shutdown()
{
}

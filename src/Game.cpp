#include "Game.hpp"

Game::Game()
: mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mLeftClicked(false)
, mRightClicked(false)
, mKeyCIsPressed(false)
, mKeyEIsPressed(false)
, mClickPos(0, 0)
{

}

bool Game::Initialize()
{
    // Set up the window
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
		// Set camera
		const auto t = mCamera.createTransformer();

		// Check for left mouse click
		mLeftClicked = MouseL.down();
		if (mLeftClicked)
		{
		    mClickPos = Cursor::PosF();
		}
		// Check for right mouse click
		mRightClicked = MouseR.down();
		if (mRightClicked)
		{
		    mClickPos = Cursor::PosF();
		}
    }
	else if (mState != GameState::isPlaying)
	{
		mKeyCIsPressed = KeyC.down();
		mKeyEIsPressed = KeyE.down();
	}
}

void Game::UpdateGame()
{
    // Update the board
    mBoard.Update(mClickPos, mLeftClicked, mRightClicked);

    // Check for game over or clear
    if (mBoard.IsGameOver())
    {
        mState = GameState::isGameOver;
    }
    else if (mBoard.IsCleared())
    {
        mState = GameState::isGameClear;
    }

	// Continue or end the game on key press
	if (mState != GameState::isPlaying)
	{
		if (mKeyCIsPressed)
		{
			mBoard.Reset();
			mBoard.CreateBoard({ 24, 16 }, 80);
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
    // Set background color
    Scene::SetBackground(Palette::Black);

    // Set camera
    const auto t = mCamera.createTransformer();

    // Draw game elements
    mBoard.Draw();

	// Draw game over or clear message
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
    // Register font
    FontAsset::Register(U"Number", FontMethod::MSDF, 48, Typeface::Bold);
    FontAsset::Register(U"Message", FontMethod::MSDF, 48, Typeface::Bold);
    // Register texture
    TextureAsset::Register(U"Mine", U"💣"_emoji);
    TextureAsset::Register(U"Flag", U"🚩"_emoji);

    // Create a board
	mBoard.CreateBoard({24, 16}, 80);
}

void Game::Shutdown()
{
}

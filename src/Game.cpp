#include "Game.hpp"

Game::Game()
: mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mPlayer(Point{0, 0})
, mLife(3)
, mHasKey(false)
, mHasPendingOpen(false)
, mPendingTargetGridPos(0, 0)
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
	else
	{
		mKeyCIsPressed = KeyC.down();
		mKeyEIsPressed = KeyE.down();
	}
}

void Game::UpdateGame()
{
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

		return;
	}

	bool wasMoving = mPlayer.GetIsMoving();
	mPlayer.Update();
	bool isMoving = mPlayer.GetIsMoving();

	// 目的地に到着した瞬間に、マスを開ける予約があったら開けて処理する
	if (wasMoving && !isMoving && mHasPendingOpen)
	{
		bool hitMine = mBoard.OpenCell(mPendingTargetGridPos);
		ProcessCellOpenResult(hitMine);
		mHasPendingOpen = false;
	}

	// 立ち止まっているときのみ、入力を受け付ける
	if (!isMoving)
	{
		if (mLeftClicked || mRightClicked)
		{
			Point targetGridPos = mBoard.GetGridPosFromScreenPos(mClickPos);

			if (mBoard.IsValidGridPos(targetGridPos))
			{
				Point playerGridPos = mPlayer.GetGridPos();

				if (mLeftClicked)
				{
					if (mBoard.CanOpen(targetGridPos, playerGridPos))
					{
						// 目的地が開けられる場合は、そこに最も近いマスと経路を取得
						Point closestPos = mBoard.GetClosestReachableGridPos(playerGridPos, targetGridPos);
						Array<Point> path = mBoard.FindPathBFS(playerGridPos, closestPos);

						// プレイヤーの周囲8マスではない場合はマスを開ける予約をする
						// 周囲8マスであれば移動がないのでそのまま開ける
						if (!path.empty())
						{
							mPlayer.SetPath(path);
							mHasPendingOpen = true;
							mPendingTargetGridPos = targetGridPos;
						}
						else
						{
							bool hitMine = mBoard.OpenCell(targetGridPos);
							ProcessCellOpenResult(hitMine);
						}
					}
					else if (mBoard.IsOpenedGridPos(targetGridPos))
					{
						// 目的地が開いているマスの場合は、移動のみ
						Array<Point> path = mBoard.FindPathBFS(playerGridPos, targetGridPos);
						if (!path.empty())
						{
							mPlayer.SetPath(path);
							mHasPendingOpen = false;
						}
					}
				}
				else if (mRightClicked)
				{
					mBoard.ToggleFlag(targetGridPos);
				}
			}
		}

		Point playerGridPos = mPlayer.GetGridPos();

		if (playerGridPos == mBoard.GetKeyGridPos() && !mHasKey)
		{
			mHasKey = true;
			mBoard.RemoveKey();
			AudioAsset(U"Key").playOneShot();
		}

		if (playerGridPos == mBoard.GetGoalGridPos() && mHasKey)
		{
			mState = GameState::isGameClear;
			AudioAsset(U"Clear").playOneShot();
		}
	}
}

void Game::GenerateOutput()
{
    Scene::SetBackground(Palette::Black);

    const auto t = mCamera.createTransformer();

	mBoard.Draw();

	// どのマスを選択しているかをフィードバック
	if (mState == GameState::isPlaying)
	{
		Point hoverGridPos = mBoard.GetGridPosFromScreenPos(Cursor::PosF());
		if (mBoard.IsValidGridPos(hoverGridPos))
		{
			Point hoverScreenPos = mBoard.GetScreenPosFromGridPos(hoverGridPos);
			Rect{ Arg::center(hoverScreenPos), 50, 50 }.draw(ColorF{1.0, 0.3});
		}
	}

	Vec2 playerScreenPos = mBoard.GetScreenPosFromGridPos(mPlayer.GetDoubleGridPos());
	mPlayer.Draw(playerScreenPos);

	for (int32 i = 0; i < mLife; ++i) TextureAsset(U"Life").scaled(0.3).draw(630 + i * 40, -390);

	if (mHasKey) TextureAsset(U"Key").scaled(0.3).draw(630, -340);

	if (mState != GameState::isPlaying)
    {
		RectF(Vec2{ -640, -450 }, Size{ 1500, 900 }).draw(ColorF{ 0.0, 0.7 });

		String resultText = (mState == GameState::isGameOver) ? U"GAME OVER" : U"GAME CLEAR";
		Color resultColor = (mState == GameState::isGameOver) ? Palette::Red : Palette::Yellow;

        FontAsset(U"Message")(resultText).drawAt(100, {0, -150}, resultColor);

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
	TextureAsset::Register(U"Human_damage", U"imgs/character_femaleAdventurer_shoveBack.png");
	TextureAsset::Register(U"Human_damage2", U"imgs/character_femaleAdventurer_shoveBack_white.png");

	AudioAsset::Register(U"Key", Resource(U"sounds/GB-Action01-09(Item).mp3"));
	AudioAsset::Register(U"Break", Resource(U"sounds/SNES-RPG01-01(Chest).mp3"));
	AudioAsset::Register(U"Walk", Resource(U"sounds/SNES-RPG01-05(Stairs).mp3"));
	AudioAsset::Register(U"Mine", Resource(U"sounds/SNES-RPG01-03(Door).mp3"));
	AudioAsset::Register(U"Clear", Resource(U"sounds/Arcade-Action01-4(Score).mp3"));
	AudioAsset::Register(U"GameOver", Resource(U"sounds/GB-Action01-06(Miss).mp3"));

	mBoard.CreateBoard({24, 16}, 80, Point{0, 3}, Point{7, 12}, Point{23, 9});
	mPlayer = Player(Point{0, 3});
}

void Game::Shutdown()
{
}

void Game::ProcessCellOpenResult(bool hitMine)
{
	if (hitMine)
	{
		--mLife;
		mPlayer.TakeDamage();
		if (mLife > 0)
		{
			AudioAsset(U"Mine").playOneShot();
		}
		else
		{
			mState = GameState::isGameOver;
			AudioAsset(U"GameOver").playOneShot();
		}
	}
	else
	{
		AudioAsset(U"Break").playOneShot();
	}
}


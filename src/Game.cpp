#include "Game.hpp"

Game::Game(const InitData& init)
: IScene{ init }
, mState(GameState::isPlaying)
, mCamera(Vec2{0, 0}, 1.0, CameraControl::None_)
, mBoard()
, mPlayer(Point{0, 0})
, mLife(3)
, mHasKey(false)
, mStopwatch{ StartImmediately::Yes }
, mScore(0)
, mHasPendingOpen(false)
, mPendingTargetGridPos(0, 0)
, mLeftClicked(false)
, mRightClicked(false)
, mKeyCIsPressed(false)
, mKeyBIsPressed(false)
, mClickPos(0, 0)
{
	mBoard.CreateBoard(getData().boardSize, getData().mineCount, getData().startGridPos, getData().keyGridPos, getData().goalGridPos);
	mPlayer = Player(getData().startGridPos);
}

void Game::update()
{
	ProcessInput();
    UpdateGame();
}

void Game::draw() const
{
    GenerateOutput();
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
		mKeyBIsPressed = KeyB.down();
	}
}

void Game::UpdateGame()
{
	if (mState != GameState::isPlaying)
	{
		if (mKeyCIsPressed)
		{
			changeScene(U"Game");
		}
		else if (mKeyBIsPressed)
		{
			changeScene(U"Title");
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
			mStopwatch.pause();
			AudioAsset(U"Clear").playOneShot();

			int32 opendCount = mBoard.GetOpenedCellCount();
			mScore = 10000 + (mLife * 2000) - (opendCount * 30);
			mScore = Max(0, mScore);
		}
	}
}

void Game::GenerateOutput() const
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

	String timeText = mStopwatch.format(U"mm:ss.xx");
	FontAsset(U"Time")(timeText).draw(630, -290, Palette::White);

	if (mState != GameState::isPlaying)
    {
		RectF(Vec2{ -640, -450 }, Size{ 1500, 900 }).draw(ColorF{ 0.0, 0.7 });

		String resultText = (mState == GameState::isGameOver) ? U"GAME OVER" : U"GAME CLEAR";
		Color resultColor = (mState == GameState::isGameOver) ? Palette::Red : Palette::Yellow;

        FontAsset(U"Message")(resultText).drawAt(100, {0, -150}, resultColor);

		if (mState == GameState::isGameClear)
		{
			FontAsset(U"Message")(U"Time: {}"_fmt(timeText)).drawAt({0, -50}, Palette::White);
			FontAsset(U"Message")(U"Score: {}"_fmt(mScore)).drawAt({0, 20}, Palette::White);
		}

		double alpha = Periodic::Sine0_1(2.0s);
        FontAsset(U"Message")(U"PRESS [C] to Continue").drawAt({0, 120}, ColorF{1.0, alpha});
        FontAsset(U"Message")(U"PRESS [B] to Return Title").drawAt({0, 190}, ColorF{1.0, alpha});
        FontAsset(U"Message")(U"PRESS [Esc] to End").drawAt({0, 260}, ColorF{1.0, alpha});
    }
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
			mStopwatch.pause();
			AudioAsset(U"GameOver").playOneShot();
		}
	}
	else
	{
		AudioAsset(U"Break").playOneShot();
	}
}


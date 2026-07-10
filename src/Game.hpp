#pragma once
#include <Siv3D.hpp>
#include "Board.hpp"
#include "Player.hpp"
#include "Common.hpp"

enum class GameState
{
    isPlaying,
    isGameOver,
    isGameClear
};

class Game : public App::Scene
{
    public:
        Game(const InitData& init);

		void update() override;
		void draw() const override;

    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput() const;

		void ProcessCellOpenResult(bool hitMine);

        GameState mState;
        Camera2D mCamera;

        Board mBoard;
		Player mPlayer;

		int32 mLife;
		bool mHasKey;
		Stopwatch mStopwatch;
		int32 mScore;

		bool mHasPendingOpen;
		Point mPendingTargetGridPos;

        bool mLeftClicked;
        bool mRightClicked;
		bool mKeyCIsPressed;
		bool mKeyBIsPressed;
        Vec2 mClickPos;
};

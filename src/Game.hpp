#pragma once
#include <Siv3D.hpp>
#include "Board.hpp"
#include "Player.hpp"

enum class GameState
{
    isPlaying,
    isGameOver,
    isGameClear
};

class Game
{
    public:
        Game();

        bool Initialize();
        void RunLoop();
        void Shutdown();
    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
        void LoadData();

        GameState mState;
        Camera2D mCamera;

        Board mBoard;
		Player mPlayer;

		int32 mLife;
		bool mHasKey;

        bool mLeftClicked;
        bool mRightClicked;
		bool mKeyCIsPressed;
		bool mKeyEIsPressed;
        Vec2 mClickPos;
};

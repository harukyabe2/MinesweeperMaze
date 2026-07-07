#pragma once
#include <Siv3D.hpp>
#include "Cell.hpp"

class Board
{
    public:
        Board();

        void Draw() const;
		void Reset();

		// 指定したマスが盤面内にあるか、すでに開いているかを判定
		bool IsValidGridPos(const Point& gridPos) const;
		bool IsOpenedGridPos(const Point& gridPos) const;

		// 対象のマスが開けるかどうかを判定
		bool CanOpen(const Point& targetGridPos, const Point& playerGridPos) const;

		void ToggleFlag(const Point& gridPos);

		// 到達可能なマスの中で、指定したマスに最も近いマスを取得
		Point GetClosestReachableGridPos(const Point& startGridPos, const Point& targetGridPos) const;

		// 盤面を生成し、地雷や特殊なマスを配置
		void CreateBoard(const Size& size, int32 mineCount, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos);

        int32 GetMineCount(const Point& gridPos);

		// スクリーン座標とマス目の相互変換
        Point GetGridPosFromScreenPos(const Vec2& screenPos);
		Point GetScreenPosFromGridPos(const Point& gridPos) const;

        void OpenCell(const Point& gridPos);

		// BFSを用いて、ゴールまでの経路が存在するか判定
		bool CheckPathBFS(const Point& startGridPos, const Point& goalGridPos);
		// BFSを用いて、スタートからゴールまでの経路を取得
		Array<Point> FindPathBFS(const Point& startGridPos, const Point& goalGridPos) const;

		// スタート、キー、ゴールの周囲8マスには地雷を配置しないようにするための判定
		bool IsSafeZone(const Point& gridPos, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos);

        bool IsCleared() const { return mSafeCellCount == 0; }
        bool IsGameOver() const { return mIsGameOver; }

    private:
        Grid<Cell> mCells;
		Array<Point> mMines;

        int32 mSafeCellCount;
        bool mIsGameOver;

		Point mStartGridPos;
		Point mKeyGridPos;
		Point mGoalGridPos;

        Array<Point> mOffsets = {
            {-1, -1}, {0, -1}, {1, -1},
            {-1, 0},           {1, 0},
            {-1, 1},  {0, 1},  {1, 1}
        };
		Array<Point> mDirections = {
			{0, -1}, {-1, 0}, {1, 0}, {0, 1}
		};
};

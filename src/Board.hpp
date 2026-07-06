#pragma once
#include <Siv3D.hpp>
#include "Cell.hpp"

class Board
{
    public:
        Board();

        void Draw() const;
		void Reset();

		bool IsValidGridPos(const Point& gridPos) const;
		bool IsOpenedGridPos(const Point& gridPos) const;
		bool CanOpen(const Point& targetGridPos, const Point& playerPos) const;
		void ToggleFlag(const Point& gridPos);

		Point GetClosestReachableGridPos(const Point& start, const Point& target) const;

        void CreateBoard(const Size& size, int32 mineCount, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos);

        int32 GetMineCount(const Point& gridPos);
        Point GetGridPosFromScreenPos(const Vec2& screenPos);
		Point GetScreenPosFromGridPos(const Point& gridPos) const;
        void OpenCell(const Point& gridPos);

		bool CheckPathBFS(const Point& start, const Point& goal);
		bool IsSafeZone(const Point& gridPos, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos);
		Array<Point> FindPathBFS(const Point& start, const Point& goal) const;

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

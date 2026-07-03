#pragma once
#include <Siv3D.hpp>
#include "Cell.hpp"

// Board class represents the game board
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

		// Create the board with the given size and mine count
        void CreateBoard(const Size& size, int32 mineCount, Point start, Point key, Point goal);
        // Get the mine count of the cell at the given position
        int32 GetMineCount(const Point& pos);
        // Get the cell grid position from the given screen position
        Point GetGridPosFromScreenPos(const Vec2& screenPos);
		// Get the screen position from the given cell grid position
		Point GetScreenPosFromGridPos(const Point& gridPos) const;
        // Open the cell at the given cell grid position
        void OpenCell(const Point& gridPos);
		// Check if there is a path from start to goal using BFS
		bool CheckPathBFS(const Point& start, const Point& goal);
		bool IsSafeZone(const Point& pos, const Point& start, const Point& key, const Point& goal);
		// Find a path from start to goal 4-directionally using BFS
		Array<Point> FindPathBFS(const Point& start, const Point& goal) const;
        // Check if the game is cleared or over
        bool IsCleared() const { return mSafeCellCount == 0; }
        bool IsGameOver() const { return mIsGameOver; }
    private:
        // The grid of cells
        Grid<Cell> mCells;
        // The offsets
        Array<Point> mOffsets = {
            {-1, -1}, {0, -1}, {1, -1},
            {-1, 0},           {1, 0},
            {-1, 1},  {0, 1},  {1, 1}
        };
		// The directions
		Array<Point> mDirections = {
			{0, -1}, {-1, 0}, {1, 0}, {0, 1}
		};
		// The mine positions
		Array<Point> mMines;
        // The count of safe cells
        int32 mSafeCellCount;
        // Whether the game is over
        bool mIsGameOver;
		// The start, key and goal positions
		Point mStartGridPos;
		Point mKeyGridPos;
		Point mGoalGridPos;
};

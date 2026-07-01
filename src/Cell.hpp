#pragma once
#include <Siv3D.hpp>

// Cell class represents a single cell
class Cell
{
    public:
        Cell();

        void Draw(const Point& pos) const;
		void Reset();

        // Getters and setters
        int32 GetMineCount() const { return mMineCount; }
        void SetMineCount(const int32 mineCount) { mMineCount = mineCount; }
        bool GetIsOpened() const { return mIsOpened; }
        void SetIsOpened(const bool isOpened) { mIsOpened = isOpened; }
        bool GetIsFlagged() const { return mIsFlagged; }
        void SetIsFlagged(const bool isFlagged) { mIsFlagged = isFlagged; }
        void SetIsExploded(const bool isExploded) { mIsExploded = isExploded; }
    private:
        // The number of mines around this cell
        int32 mMineCount;
        // Whether this cell is opened
        bool mIsOpened;
        // Whether this cell is flagged
        bool mIsFlagged;
		// Whether mine is exploded
		bool mIsExploded;
};

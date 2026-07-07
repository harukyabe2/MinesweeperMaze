#pragma once
#include <Siv3D.hpp>

class Player
{
	public:
		Player(const Point& startGridPos);

		void Update();
		void Draw(const Vec2& screenPos) const;

		void SetPath(const Array<Point>& path);
		Vec2 GetDoubleGridPos() const { return mDoubleGridPos; }

		Point GetGridPos() const { return mGridPos; }
		void SetGridPos(const Point& gridPos) { mGridPos = gridPos;  }

	private:
		Point mGridPos;
		Vec2 mDoubleGridPos;

		bool mIsMoving;

		Array<Point> mPath;
		size_t mCurrentPathIndex;
};

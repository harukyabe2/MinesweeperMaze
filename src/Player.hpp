#pragma once
#include <Siv3D.hpp>

class Player
{
	public:
		Player(const Point& startGridPos);

		void Update();
		void Draw(const Vec2& screenPos) const;

		void SetPath(const Array<Point>& path);

		void TakeDamage();

		Point GetGridPos() const { return mGridPos; }
		void SetGridPos(const Point& gridPos) { mGridPos = gridPos;  }
		Vec2 GetDoubleGridPos() const { return mDoubleGridPos; }
		bool GetIsMoving() const { return mIsMoving; }

	private:
		Point mGridPos;
		Vec2 mDoubleGridPos;

		bool mIsMoving;

		Array<Point> mPath;
		size_t mCurrentPathIndex;

		Stopwatch mDamageTimer;
};

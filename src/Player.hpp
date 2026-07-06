#pragma once
#include <Siv3D.hpp>

class Player
{
	public:
		Player(const Point& startGridPos);

		void Update();
		void Draw(const Point& screenPos) const;

		// Getters and setters
		Point GetGridPos() const { return mGridPos; }
		void SetGridPos(const Point& gridPos) { mGridPos = gridPos;  }

	private:
		Point mGridPos;
		bool mHasKey;
};

#pragma once
#include <Siv3D.hpp>

class Player
{
	public:
		Player(const Point start);

		void Update();
		void Draw(const Point screenPos) const;

		//// Check if the player can move to the target position
		//bool CanMoveTo(const Point& targetGridPos) const;
		//// If the target position is adjacent to the current position, move the player and return true
		//bool TryMove(const Point& targetGridPos);

		// Getters and setters
		Point GetGridPos() const { return mGridPos; }
		void SetGridPos(const Point& gridPos) { mGridPos = gridPos;  }
	private:
		// The current position of the player
		Point mGridPos;
		// If the player has the key
		bool mHasKey;
};

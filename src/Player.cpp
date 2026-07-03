#include "Player.hpp"

Player::Player(const Point start)
: mGridPos(start)
, mHasKey(false)
{
}

void Player::Update()
{
}

void Player::Draw(const Point screenPos) const
{
	TextureAsset(U"Cat").scaled(0.3).drawAt(screenPos);
}

//bool Player::CanMoveTo(const Point& targetGridPos) const
//{
//	int32 dx = Abs(targetGridPos.x - mGridPos.x);
//	int32 dy = Abs(targetGridPos.y - mGridPos.y);
//
//	return (dx <= 1 && dy <= 1 && (dx != 0 || dy != 0));
//}
//
//bool Player::TryMove(const Point& targetGridPos)
//{
//	if (CanMoveTo(targetGridPos))
//	{
//		mGridPos = targetGridPos;
//		return true;
//	}
//	return false;
//}

#include "Player.hpp"

Player::Player(const Point& startGridPos)
: mGridPos(startGridPos)
, mHasKey(false)
{
}

void Player::Update()
{
}

void Player::Draw(const Point& screenPos) const
{
	TextureAsset(U"Cat").scaled(0.3).drawAt(screenPos);
}


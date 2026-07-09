#include "Player.hpp"

Player::Player(const Point& startGridPos)
: mGridPos(startGridPos)
, mDoubleGridPos(startGridPos.x, startGridPos.y)
, mIsMoving(false)
, mCurrentPathIndex(0)
, mDamageTimer(0.5s)
{
}

void Player::Update()
{
	if (mDamageTimer.isRunning() && mDamageTimer > 0.5s) mDamageTimer.reset();

	if (mCurrentPathIndex < mPath.size())
	{
		mIsMoving = true;

		Vec2 targetGridPos = mPath[mCurrentPathIndex];

		double speed = 20.0 * Scene::DeltaTime();
		double distance = mDoubleGridPos.distanceFrom(targetGridPos);

		if (distance <= speed)
		{
			mDoubleGridPos = targetGridPos;
			mGridPos = mPath[mCurrentPathIndex];
			++mCurrentPathIndex;
		}
		else
		{
			mDoubleGridPos += (targetGridPos - mDoubleGridPos).normalized() * speed;
		}
	}
	else
	{
		mIsMoving = false;
		mPath.clear();
		mCurrentPathIndex = 0;
	}
}

void Player::Draw(const Vec2& screenPos) const
{
	if (mDamageTimer.isRunning())
	{
		if (Periodic::Square0_1(0.1s)) TextureAsset(U"Human_damage").resized(60).drawAt(screenPos.x, screenPos.y - 5);
		else TextureAsset(U"Human_damage2").resized(60).drawAt({screenPos.x, screenPos.y - 5});
	}
	else
	{
		if (mIsMoving)
		{
			if (Periodic::Square0_1(0.3s)) TextureAsset(U"Human_walk1").resized(60).drawAt(screenPos.x, screenPos.y - 5);
			else TextureAsset(U"Human_walk2").resized(60).drawAt(screenPos.x, screenPos.y - 5);
		}
		else
		{
			TextureAsset(U"Human_stand").resized(60).drawAt(screenPos.x, screenPos.y - 5);
		}
	}
}

void Player::SetPath(const Array<Point>& path)
{
	mPath = path;
	mCurrentPathIndex = 0;
}

void Player::TakeDamage()
{
	mDamageTimer.restart();
}

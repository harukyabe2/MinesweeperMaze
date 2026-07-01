#include "Cell.hpp"

Cell::Cell()
: mMineCount(0)
, mIsOpened(false)
, mIsFlagged(false)
, mIsExploded(false)
{
}

void Cell::Draw(const Point& pos) const
{
    if (mIsOpened)
    {
        Rect{Arg::center(pos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Gray);
        if (mMineCount > 0)
        {
            HSV color = {220.0 / mMineCount, 1.0, 1.0 - mMineCount / 9};
            FontAsset(U"Number")(U"{}"_fmt(mMineCount)).drawAt(40, pos, color);
        }
        else if (mMineCount == -1)
        {
			if (mIsExploded)
			{
				Rect{Arg::center(pos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Red);
			}
            TextureAsset(U"Mine").scaled(0.3).drawAt(pos);
        }
    }
    else if (!mIsOpened)
    {
        Rect{Arg::center(pos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Darkgray);
        if (mIsFlagged)
        {
            TextureAsset(U"Flag").scaled(0.3).drawAt(pos);
        }
    }
}

void Cell::Reset()
{
	mMineCount = 0;
	mIsOpened = false;
	mIsFlagged = false;
	mIsExploded = false;
}

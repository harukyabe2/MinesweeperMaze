#include "Cell.hpp"

Cell::Cell()
: mMineCount(0)
, mIsOpened(false)
, mIsFlagged(false)
, mIsExploded(false)
, mRole(CellRole::Normal)
{
}

void Cell::Draw(const Point& screenPos) const
{
    if (mIsOpened)
    {
        Rect{Arg::center(screenPos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Gray);

        if (mMineCount > 0)
        {
            HSV color = {220.0 / mMineCount, 1.0, 1.0 - mMineCount / 9};
            FontAsset(U"Number")(U"{}"_fmt(mMineCount)).drawAt(40, screenPos, color);
        }
        else if (mMineCount == -1)
        {
			if (mIsExploded)
			{
				Rect{Arg::center(screenPos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Red);
			}
            TextureAsset(U"Mine").scaled(0.3).drawAt(screenPos);
        }

		if (mRole == CellRole::Start)
		{
		    Rect{Arg::center(screenPos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Whitesmoke);
		}
		else if (mRole == CellRole::Key)
		{
			TextureAsset(U"Key").scaled(0.3).drawAt(screenPos);
		}
		else if (mRole == CellRole::Goal)
		{
			TextureAsset(U"Goal").scaled(0.3).drawAt(screenPos);
		}
	}
    else
    {
        Rect{Arg::center(screenPos), 50, 50}.drawFrame(1, Palette::Black).draw(Palette::Darkgray);
        if (mIsFlagged)
        {
            TextureAsset(U"Flag").scaled(0.3).drawAt(screenPos);
        }
    }

}

void Cell::Reset()
{
	mMineCount = 0;
	mIsOpened = false;
	mIsFlagged = false;
	mIsExploded = false;
	mRole = CellRole::Normal;
}

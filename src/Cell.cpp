#include "Cell.hpp"

Cell::Cell()
: mMineCount(0)
, mIsOpened(false)
, mIsFlagged(false)
, mIsExploded(false)
, mRole(CellRole::Normal)
{
	double r = Random();
	if (r < 0.05) mTextureIndex = 0;
	else if (r < 0.10) mTextureIndex = 1;
	else if (r < 0.80) mTextureIndex = 2;
	else mTextureIndex = 3;
}

void Cell::Draw(const Point& screenPos) const
{
    if (mIsOpened)
    {
        Rect{Arg::center(screenPos), 50, 50}.drawFrame(1, ColorF{0.15});

		TextureAsset(U"Wall").resized(50).drawAt(screenPos, ColorF{0.7});

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
		if (mTextureIndex == 3) TextureAsset(U"Block1").resized(50).drawAt(screenPos, ColorF{0.8});
		else if (mTextureIndex == 2) TextureAsset(U"Block1").resized(50).drawAt(screenPos);
		else if (mTextureIndex == 1) TextureAsset(U"Block2").resized(50).drawAt(screenPos);
		else TextureAsset(U"Block3").resized(50).drawAt(screenPos, ColorF{0.9});

        if (mIsFlagged)
        {
            TextureAsset(U"Flag").scaled(0.3).drawAt(screenPos);
        }
    }

}

void Cell::Reset()
{
	mMineCount = 0;
	double r = Random();
	if (r < 0.6) mTextureIndex = 0;
	else if (r < 0.8) mTextureIndex = 1;
	else mTextureIndex = 2;
	mIsOpened = false;
	mIsFlagged = false;
	mIsExploded = false;
	mRole = CellRole::Normal;
}

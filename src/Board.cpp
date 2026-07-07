#include "Board.hpp"

Board::Board()
: mSafeCellCount(0)
, mIsGameOver(false)
, mStartGridPos(0, 0)
, mKeyGridPos(0, 0)
, mGoalGridPos(0, 0)
{
}

void Board::Draw() const
{
    const Size size = mCells.size();
    for (int32 y = 0; y < size.y; ++y)
    {
        for (int32 x = 0; x < size.x; ++x)
        {
			Point screenPos = GetScreenPosFromGridPos(Point{x, y});
            mCells[Point{x, y}].Draw(screenPos);
        }
    }
}

void Board::Reset()
{
	mCells.clear();
	mMines.clear();
	mSafeCellCount = 0;
	mIsGameOver = false;
}

bool Board::IsValidGridPos(const Point& gridPos) const
{
	return (gridPos.x >= 0 && gridPos.x < mCells.width() &&
			gridPos.y >= 0 && gridPos.y < mCells.height());
}

bool Board::IsOpenedGridPos(const Point& gridPos) const
{
	return mCells[gridPos].GetIsOpened();
}

bool Board::CanOpen(const Point& targetGridPos, const Point& playerGridPos) const
{
	if (!IsValidGridPos(targetGridPos) || IsOpenedGridPos(targetGridPos) || mCells[targetGridPos].GetIsFlagged())
	{
		return false;
	}

	for (const auto& offset : mOffsets)
	{
		const Point neighborPos = targetGridPos + offset;
		if (IsValidGridPos(neighborPos) && IsOpenedGridPos(neighborPos))
		{
			if (neighborPos == playerGridPos) return true;
			if (!FindPathBFS(playerGridPos, neighborPos).empty()) return true;
		}
	}

	return false;
}

void Board::ToggleFlag(const Point& gridPos)
{
    if (!IsOpenedGridPos(gridPos))
	{
        mCells[gridPos].SetIsFlagged(!mCells[gridPos].GetIsFlagged());
    }
}

Point Board::GetClosestReachableGridPos(const Point& startGridPos, const Point& targetGridPos) const
{
	if (IsOpenedGridPos(targetGridPos) && (startGridPos == targetGridPos || !FindPathBFS(startGridPos, targetGridPos).empty()))
	{
		return targetGridPos;
	}

	Point closestPos = startGridPos;
	size_t minPathLength = 999999;

	for (const auto& offset : mOffsets)
	{
		const Point neighborPos = targetGridPos + offset;
		if (IsValidGridPos(neighborPos) && IsOpenedGridPos(neighborPos))
		{
			if (neighborPos == startGridPos)
			{
				if (0 < minPathLength)
				{
					minPathLength = 0;
					closestPos = startGridPos;
				}
				continue;
			}

			Array<Point> path = FindPathBFS(startGridPos, neighborPos);
			if (!path.empty() && path.size() < minPathLength)
			{
				minPathLength = path.size();
				closestPos = neighborPos;
			}
		}
	}

	return closestPos;
}

void Board::CreateBoard(const Size& size, int32 mineCount, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos)
{
	mStartGridPos = startGridPos;
	mKeyGridPos = keyGridPos;
	mGoalGridPos = goalGridPos;

	while (true)
	{
		mCells = Grid<Cell>(size);
		mMines.clear();
		mMines.reserve(mineCount);

		mCells[mStartGridPos].SetRole(CellRole::Start);
		mCells[mKeyGridPos].SetRole(CellRole::Key);
		mCells[mGoalGridPos].SetRole(CellRole::Goal);

		int32 currentMineCount = mineCount;

		while (currentMineCount > 0)
		{
		    const Point gridPos = {Random(0, size.x - 1), Random(0, size.y - 1)};

		    if (mCells[gridPos].GetMineCount() == 0 && !IsSafeZone(gridPos, mStartGridPos, mKeyGridPos, mGoalGridPos))
		    {
		        mCells[gridPos].SetMineCount(-1);
				mMines.push_back(gridPos);
		        --currentMineCount;
		    }
		}

		// 必ずスタートからキー、キーからゴールまでの経路が存在するようにする
		if (CheckPathBFS(mStartGridPos, mKeyGridPos) && CheckPathBFS(mKeyGridPos, mGoalGridPos))
		{
			break;
		}
	}

    for (int32 y = 0; y < size.y; ++y)
    {
        for (int32 x = 0; x < size.x; ++x)
        {
            const Point gridPos = {x, y};
            mCells[gridPos].SetMineCount(GetMineCount(gridPos));
        }
    }

	mSafeCellCount = size.x * size.y - mineCount;

	mCells[mStartGridPos].SetIsOpened(true);
	mCells[mKeyGridPos].SetIsOpened(true);
	mCells[mGoalGridPos].SetIsOpened(true);
	mSafeCellCount -= 3;
}

int32 Board::GetMineCount(const Point& gridPos)
{
    if (mCells[gridPos].GetMineCount() == -1) return -1;
    
    int32 mineCount = 0;
    for (const auto& offset : mOffsets)
    {
        const Point neighborPos = gridPos + offset;

        if (!IsValidGridPos(neighborPos)) continue;
        if (mCells[neighborPos].GetMineCount() == -1) ++mineCount;
        
    }
    return mineCount;
}

Point Board::GetGridPosFromScreenPos(const Vec2& screenPos)
{
    Point gridPos;
    gridPos.x = static_cast<int>(Math::Floor((screenPos.x + (50 * mCells.width()) / 2.0) / 50.0));
    gridPos.y = static_cast<int>(Math::Floor((screenPos.y + (50 * mCells.height()) / 2.0) / 50.0));
    return gridPos;
}

Point Board::GetScreenPosFromGridPos(const Point& gridPos) const
{
    const Size size = mCells.size();
    return Point{25 + 50 * gridPos.x - (50 * size.x) / 2, 25 + 50 * gridPos.y - (50 * size.y) / 2};
}

void Board::OpenCell(const Point& gridPos)
{
    if (!IsValidGridPos(gridPos) ||
        mCells[gridPos].GetIsOpened() ||
        mCells[gridPos].GetIsFlagged())
    {
        return;
    }

    mCells[gridPos].SetIsOpened(true);
    --mSafeCellCount;

	// 周囲8マスに地雷がない場合は、再帰的に周囲8マスを開く
	if (mCells[gridPos].GetMineCount() == 0)
    {
        for (const auto& offset : mOffsets)
        {
            const Point neighborPos = gridPos + offset;
            OpenCell(neighborPos);
        }
    }
    else if (mCells[gridPos].GetMineCount() == -1)
    {
		mCells[gridPos].SetIsExploded(true);
		for (const auto& minePos : mMines)
		{
			if (minePos != gridPos)
			{
				mCells[minePos].SetIsOpened(true);
			}
		}
        mIsGameOver = true;
    }
}

bool Board::CheckPathBFS(const Point& startGridPos, const Point& goalGridPos)
{
	bool result = false;

	Grid<int32> visited(mCells.size(), 0);
	std::queue<Point> que;
	que.push(startGridPos);
	visited[startGridPos] = 1;

	while (!que.empty())
	{
		Point gridPos = que.front();
		que.pop();

		if (gridPos == goalGridPos) return true;
		
		for (const auto& dir : mDirections)
		{
			const Point neighborPos = gridPos + dir;

			if (!IsValidGridPos(neighborPos)) continue;

			if (mCells[neighborPos].GetMineCount() != -1 && !visited[neighborPos])
			{
				visited[neighborPos] = 1;
				que.push(neighborPos);
			}
		}
	}

	return result;
}

bool Board::IsSafeZone(const Point& gridPos, const Point& startGridPos, const Point& keyGridPos, const Point& goalGridPos)
{
	if (Abs(gridPos.x - startGridPos.x) <= 1 && Abs(gridPos.y - startGridPos.y) <= 1) return true;
	if (Abs(gridPos.x - keyGridPos.x) <= 1 && Abs(gridPos.y - keyGridPos.y) <= 1) return true;
	if (Abs(gridPos.x - goalGridPos.x) <= 1 && Abs(gridPos.y - goalGridPos.y) <= 1) return true;
	return false;
}

Array<Point> Board::FindPathBFS(const Point& startGridPos, const Point& goalGridPos) const
{
	Array<Point> path;
	if (startGridPos == goalGridPos) return path;
	if (!IsValidGridPos(startGridPos) || !IsValidGridPos(goalGridPos)) return path;
	if (!IsOpenedGridPos(startGridPos) || !IsOpenedGridPos(goalGridPos)) return path;

	Grid<Point> parent(mCells.size(), Point{-1, -1});
	std::queue<Point> que;
	que.push(startGridPos);
	parent[startGridPos] = startGridPos;

	bool found = false;
	while (!que.empty())
	{
		Point gridPos = que.front();
		que.pop();

		if (gridPos == goalGridPos)
		{
			found = true;
			break;
		}

		for (const auto& dir : mDirections)
		{
			Point nextPos = gridPos + dir;
			if (IsValidGridPos(nextPos) && IsOpenedGridPos(nextPos) && parent[nextPos] == Point{-1, -1})
			{
				parent[nextPos] = gridPos;
				que.push(nextPos);
			}
		}
	}

	if (found)
	{
		Point current = goalGridPos;
		while (current != startGridPos)
		{
			path.push_back(current);
			current = parent[current];
		}
		path.reverse();
	}

	return path;
}





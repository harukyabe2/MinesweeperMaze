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

bool Board::CanOpen(const Point& targetGridPos, const Point& playerPos) const
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
			if (neighborPos == playerPos) return true;
			if (!FindPathBFS(playerPos, neighborPos).empty()) return true;
		}
	}

	return false;
}

void Board::ToggleFlag(const Point& gridPos)
{
    if (!mCells[gridPos].GetIsOpened())
	{
        mCells[gridPos].SetIsFlagged(!mCells[gridPos].GetIsFlagged());
    }
}

void Board::CreateBoard(const Size& size, int32 mineCount, Point start, Point key, Point goal)
{
	mStartGridPos = start;
	mKeyGridPos = key;
	mGoalGridPos = goal;

	while (true)
	{
		mCells = Grid<Cell>(size);
		mMines.clear();
		// avoid reallocation
		mMines.reserve(mineCount);

		mCells[mStartGridPos].SetRole(CellRole::Start);
		mCells[mKeyGridPos].SetRole(CellRole::Key);
		mCells[mGoalGridPos].SetRole(CellRole::Goal);

		int32 currentMineCount = mineCount;

		// Randomly place mines
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

		if (CheckPathBFS(mStartGridPos, mKeyGridPos) && CheckPathBFS(mKeyGridPos, mGoalGridPos))
		{
			break;
		}
	}

    // Calculate mine counts for each cell
    for (int32 y = 0; y < size.y; ++y)
    {
        for (int32 x = 0; x < size.x; ++x)
        {
            const Point pos = {x, y};
            mCells[pos].SetMineCount(GetMineCount(pos));
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
    // If this cell is a mine, return -1
    if (mCells[gridPos].GetMineCount() == -1)
    {
        return -1;
    }
    
    int32 mineCount = 0;
    // Check all 8 neighbors
    for (const auto& offset : mOffsets)
    {
        const Point neighborPos = gridPos + offset;
        if (neighborPos.x < 0 || neighborPos.x >= mCells.width() ||
            neighborPos.y < 0 || neighborPos.y >= mCells.height())
        {
            continue;
        }
        if (mCells[neighborPos].GetMineCount() == -1)
        {
            ++mineCount;
        }
    }
    return mineCount;
}

Point Board::GetGridPosFromScreenPos(const Vec2& screenPos)
{
    Point idx;
    idx.x = static_cast<int>(Math::Floor((screenPos.x + (50 * mCells.width()) / 2.0) / 50.0));
    idx.y = static_cast<int>(Math::Floor((screenPos.y + (50 * mCells.height()) / 2.0) / 50.0));
    return idx;
}

Point Board::GetScreenPosFromGridPos(const Point& gridPos) const
{
    const Size size = mCells.size();
    return Point{25 + 50 * gridPos.x - (50 * size.x) / 2, 25 + 50 * gridPos.y - (50 * size.y) / 2};
}

void Board::OpenCell(const Point& gridPos)
{
	// If the position is out of bounds, or the cell is already opened or flagged, do nothing
    if (gridPos.x < 0 || gridPos.x >= mCells.width() ||
        gridPos.y < 0 || gridPos.y >= mCells.height() ||
        mCells[gridPos].GetIsOpened() ||
        mCells[gridPos].GetIsFlagged())
    {
        return;
    }
    mCells[gridPos].SetIsOpened(true);
    --mSafeCellCount;

	// If this cell has no mines around, open all its neighbors recursively
	// If this cell is a mine, set it as exploded and open all mines, then end the game
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

bool Board::CheckPathBFS(const Point& start, const Point& goal)
{
	bool result = false;

	// Queue for searching the path
	Grid<int32> visited(mCells.size(), 0);
	std::queue<Point> que;
	que.push(start);
	visited[start] = 1;

	while (!que.empty())
	{
		Point gridPos = que.front();
		que.pop();
		if (gridPos == goal)
		{
			return true;
		}

		for (const auto& dir : mDirections)
		{
			const Point neighborPos = gridPos + dir;
			if (neighborPos.x < 0 || neighborPos.x >= mCells.width() ||
			    neighborPos.y < 0 || neighborPos.y >= mCells.height())
			{
			    continue;
			}

			if (mCells[neighborPos].GetMineCount() != -1 && !visited[neighborPos])
			{
				visited[neighborPos] = 1;
				que.push(neighborPos);
			}
		}
	}

	return result;
}

bool Board::IsSafeZone(const Point& gridPos, const Point& start, const Point& key, const Point& goal)
{
	if (Abs(gridPos.x - start.x) <= 1 && Abs(gridPos.y - start.y) <= 1) return true;
	if (Abs(gridPos.x - key.x) <= 1 && Abs(gridPos.y - key.y) <= 1) return true;
	if (Abs(gridPos.x - goal.x) <= 1 && Abs(gridPos.y - goal.y) <= 1) return true;
	return false;
}

Array<Point> Board::FindPathBFS(const Point& start, const Point& goal) const
{
	Array<Point> path;
	if (start == goal) return path;
	if (!IsValidGridPos(start) || !IsValidGridPos(goal)) return path;
	if (!IsOpenedGridPos(start) || !IsOpenedGridPos(goal)) return path;

	Grid<Point> parent(mCells.size(), Point{-1, -1});
	std::queue<Point> que;
	que.push(start);
	parent[start] = start;

	bool found = false;
	while (!que.empty())
	{
		Point gridPos = que.front();
		que.pop();

		if (gridPos == goal)
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
		Point current = goal;
		while (current != start)
		{
			path.push_front(current);
			current = parent[current];
		}
	}

	return path;
}





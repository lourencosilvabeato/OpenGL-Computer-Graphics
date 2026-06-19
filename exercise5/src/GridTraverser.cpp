// This source code is property of the Computer Graphics and Visualization
// chair of the TU Dresden. Do not distribute!
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "GridTraverser.h"
#include "GridUtils.h"

GridTraverser::GridTraverser()
{
}

GridTraverser::GridTraverser(const Eigen::Vector3f &o, const Eigen::Vector3f &d, const Eigen::Vector3f cell_extents)
		: orig(o), dir(d), cellExtents(cell_extents)
{
	dir.normalize();
	Init();
}

Eigen::Vector3f &GridTraverser::Origin()
{
	return orig;
}
const Eigen::Vector3f &GridTraverser::Origin() const
{
	return orig;
}

Eigen::Vector3f &GridTraverser::Direction()
{
	return dir;
}

const Eigen::Vector3f &GridTraverser::Direction() const
{
	return dir;
}

void GridTraverser::SetCellExtents(const Eigen::Vector3f &cellExtent)
{
	this->cellExtents = cellExtent;
	Init();
}

void GridTraverser::Init()
{
	current = PositionToCellIndex(orig, cellExtents);

	for (int i = 0; i < 3; i++)
	{
		if (dir[i] > 0)
			step[i] = 1;
		else if (dir[i] < 0)
			step[i] = -1;
		else
			step[i] = 0;

		if (step[i] != 0)
			tDelta[i] = cellExtents[i] / std::abs(dir[i]);
		else
			tDelta[i] = std::numeric_limits<float>::infinity();

		float cellBorder;
		if (step[i] > 0)
			cellBorder = (current[i] + 1) * cellExtents[i];
		else
			cellBorder = current[i] * cellExtents[i];

		if (step[i] != 0)
			tMax[i] = (cellBorder - orig[i]) / dir[i];
		else
			tMax[i] = std::numeric_limits<float>::infinity();
	}
}

void GridTraverser::operator++(int)
{
	int axis = 0;
	if (tMax[1] < tMax[axis])
		axis = 1;
	if (tMax[2] < tMax[axis])
		axis = 2;

	current[axis] += step[axis];

	tMax[axis] += tDelta[axis];
}

Eigen::Vector3i GridTraverser::operator*()
{
	return current;
}

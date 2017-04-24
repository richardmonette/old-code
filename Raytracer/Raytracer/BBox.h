#pragma once

#include "Point.h"
#include <algorithm>

using namespace std;

class BBox
{
public:
	BBox();

	BBox(const Point &p) : pMin(p), pMax(p) {}
	BBox(const Point &p1, const Point &p2)
	{
		pMin = Point(min(p1.x, p2.x),
					 min(p1.y, p2.y),
				     min(p1.z, p2.z));
		pMax = Point(max(p1.x, p2.x),
					 max(p1.y, p2.y),
					 max(p1.z, p2.z));
	}

	~BBox();

	Point pMin, pMax;
};

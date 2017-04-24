#include "BBox.h"

BBox::BBox()
{
	pMin = Point(INFINITY, INFINITY, INFINITY);
	pMax = Point(-INFINITY, -INFINITY, -INFINITY);
}

BBox::~BBox()
{
}

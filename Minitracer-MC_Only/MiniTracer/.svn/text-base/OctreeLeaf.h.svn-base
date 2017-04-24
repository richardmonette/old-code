#pragma once

#include <list>

#include "Global.h"

static const int maxobj = 8;

using namespace std;

class OctreeLeaf
{
public:
	OctreeLeaf(BoundingBox *containerBox, int depth);
	~OctreeLeaf(void);

	void Split();
	void Distribute();

	void IntersectTriangles(Ray &ray, double &bestIntersectDistance, Vec &bestIntersectPosition, Vec &bestIntersectNormal, Triangle &intersectTriangle);

	list<Triangle*> containedTriangles;
	OctreeLeaf *childLeaves[8];
	
	BoundingBox containerBox;

	int depth;
};

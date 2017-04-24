#pragma once

#include "Global.h"
#include "OctreeLeaf.h"

class Octree : public OctreeLeaf
{
public:
	Octree();
	~Octree();

	void ParseScene( Triangle *triangles, int numTriangles );

};

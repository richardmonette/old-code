#pragma once

#include "Vector3.h"
#include "Voxel.h"

#include <set>
#include <vector>

const int MAX_OCTREE_DEPTH = 100;
const int MIN_VOXELS_PER_OCTREE = 1;
const int MAX_VOXELS_PER_OCTREE = 8;

class Octree
{
public:

    Vector3 corner1; //(minX, minY, minZ)
    Vector3 corner2; //(maxX, maxY, maxZ)
    Vector3 center;  //((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)

    Octree *children[2][2][2];

	Vector3 color;

    bool hasChildren;

    vector<Voxel*> voxels;

    int depth;

	Octree(Vector3 c1, Vector3 c2, int d);
	~Octree();

	void Draw(int currentDepth, Vector3 cameraPosition);
	Vector3 ComputeColor();

	void addEntry(Voxel *voxel);
	void haveChildren();
	void add(Voxel* voxel);
	void destroyChildren();
};


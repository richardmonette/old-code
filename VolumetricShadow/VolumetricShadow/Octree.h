#pragma once

#include <vectormath_aos.h>

#include "ray.h"
#include "Types.h"

using namespace Vectormath::Aos;

#define MAX_VOXELS_PER_OCTREE 10

class Octree
{
public:

	Octree(Vector3 c1, Vector3 c2, Octree *p);
	~Octree();

	long Adfs();

	void Intersect(const Ray &r, int *pcount);

	void AddEntry(Vector3 *voxel);
	void HaveChildren();
	void Add(Vector3 *voxel);
	void DestroyChildren();

	bool hasChildren;

	int numVoxels;
	int avgNumVoxels;

	Vector3 p[2];
    Vector3 center;

	Octree *parent;

    Octree *children[2][2][2];

    Vector3 *voxels[MAX_VOXELS_PER_OCTREE];
};


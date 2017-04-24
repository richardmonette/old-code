#pragma once

#include "Vector3.h"

#include <set>
#include <vector>

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 3;
const int MAX_BALLS_PER_OCTREE = 6;

struct IrrCacheEntry;

class Octree
{
public:
        Vector3 corner1; //(minX, minY, minZ)
        Vector3 corner2; //(maxX, maxY, maxZ)
        Vector3 center;//((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)

		/* The children of this, if this has any.  children[0][*][*] are the
         * children with x coordinates ranging from minX to centerX.
         * children[1][*][*] are the children with x coordinates ranging from
         * centerX to maxX.  Similarly for the other two dimensions of the
         * children array.
         */
        Octree *children[2][2][2];

		//Whether this has children
        bool hasChildren;
        //The balls in this, if this doesn't have any children
        vector<IrrCacheEntry*> cacheEntries;
        //The depth of this in the tree
        int depth;
        //The number of balls in this, including those stored in its children
        int numCacheEntries;


	Octree(Vector3 c1, Vector3 c2, int d);
	~Octree();

	void addEntry(IrrCacheEntry *cacheEntry);
	void haveChildren();
	void add(IrrCacheEntry* cacheEntry);
	void destroyChildren();
};


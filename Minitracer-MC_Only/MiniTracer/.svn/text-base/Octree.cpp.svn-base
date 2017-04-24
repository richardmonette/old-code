#include "Octree.h"
#include <list>

using namespace std;

Octree::Octree()
: OctreeLeaf(new BoundingBox(), 0)
{
}

Octree::~Octree(void)
{
}

void Octree::ParseScene( Triangle *triangles, int numTriangles )
{
	for (int i = 0; i < numTriangles; i++)
	{
		Triangle tri = triangles[i];

		for (int k = 0; k < 3; k++)
		{
			containerBox.Expand( &tri.vertices[k] );
		}

		containedTriangles.push_back(&triangles[i]);
	}

    Distribute();
}


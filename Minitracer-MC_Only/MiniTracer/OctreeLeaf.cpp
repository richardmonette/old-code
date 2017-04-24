#include "OctreeLeaf.h"
#include "Global.h"

OctreeLeaf::OctreeLeaf(BoundingBox *_containerBox, int d)
{
	containerBox.pMin.x = _containerBox->pMin.x;
	containerBox.pMin.y = _containerBox->pMin.y;
	containerBox.pMin.z = _containerBox->pMin.z;

	containerBox.pMax.x = _containerBox->pMax.x;
	containerBox.pMax.y = _containerBox->pMax.y;
	containerBox.pMax.z = _containerBox->pMax.z;

	depth = d;
}

OctreeLeaf::~OctreeLeaf(void)
{
}

void OctreeLeaf::Split()
{
	Vec unitX = Vec(1,0,0);
	Vec unitY = Vec(0,1,0);
	Vec unitZ = Vec(0,0,1);

	Vec half = containerBox.pMax - containerBox.pMin;
	half *= 0.5f;
	Vec halfx = unitX.mult(half);
	Vec halfy = unitY.mult(half);
	Vec halfz = unitZ.mult(half);

    childLeaves[0] = new OctreeLeaf( new BoundingBox( containerBox.pMin, containerBox.pMin + half ), depth+1 );
    childLeaves[1] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfx, containerBox.pMax - half + halfx ), depth+1 );
    childLeaves[2] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfz, containerBox.pMin + half + halfz ), depth+1 );
    childLeaves[3] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfx + halfz, containerBox.pMax - halfy ), depth+1 );
    childLeaves[4] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfy, containerBox.pMax - halfx - halfz ), depth+1 );
    childLeaves[5] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfy + halfx, containerBox.pMax - halfz ), depth+1 );
    childLeaves[6] = new OctreeLeaf( new BoundingBox( containerBox.pMin + halfy + halfz, containerBox.pMax - halfx ), depth+1 );
    childLeaves[7] = new OctreeLeaf( new BoundingBox( containerBox.pMin + half, containerBox.pMax ), depth+1 );
}

void OctreeLeaf::Distribute()
{
	if ( containedTriangles.size() > maxobj )
    {
        Split();
		list<Triangle*>::iterator triIt;
		for ( triIt = containedTriangles.begin(); triIt != containedTriangles.end();)
        {
			bool childLeafContainsTriangle = false;
			for ( int i = 0; i < 8; i++)
            {
                if ( childLeaves[i]->containerBox.Contains( &(*triIt)->boundingBox ) )
                {
                    childLeaves[i]->containedTriangles.push_back( (*triIt) );
                    triIt = containedTriangles.erase( triIt );
					childLeafContainsTriangle = true;
                    break;
                }
            }

			if (!childLeafContainsTriangle) 
			{
				triIt++;
			}
        }

		for ( int i = 0; i < 8; i++)
        {
            childLeaves[i]->Distribute();
        }
    }
}

void OctreeLeaf::IntersectTriangles(Ray &ray, double &bestIntersectDistance, Vec &bestIntersectPosition, Vec &bestIntersectNormal, Triangle &intersectTriangle)
{
	double currentIntersectDistance;
	Vec currentIntersectPosition;
	Vec currentIntersectNormal;

    list<Triangle*>::iterator triIt;
	for ( triIt = containedTriangles.begin(); triIt != containedTriangles.end(); triIt++)
    {
        if( (*triIt)->Intersect(ray.origin, ray.direction, currentIntersectDistance) )
		{
			currentIntersectPosition = ray.origin + ray.direction * currentIntersectDistance;
			currentIntersectNormal = (*triIt)->getNormal();//(*triIt)->GetNormalBarycentric(&currentIntersectNormal, u, v);

			if (currentIntersectDistance < bestIntersectDistance)
            {				
				bestIntersectDistance = currentIntersectDistance;
				bestIntersectPosition = currentIntersectPosition;
				bestIntersectNormal =   currentIntersectNormal;

				intersectTriangle = *(*triIt);
			}
		}
    }

	for ( int i = 0; i < 8; i++)
    {
		if ( childLeaves[i]->containerBox.Intersects( ray ) )
		{
			childLeaves[i]->IntersectTriangles(ray, bestIntersectDistance, bestIntersectPosition, bestIntersectNormal, intersectTriangle);
		}
	}
}


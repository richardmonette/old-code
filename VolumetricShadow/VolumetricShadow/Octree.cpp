
#include "Octree.h"

#include <stdlib.h>

Octree::Octree(Vector3 c1, Vector3 c2, Octree *pr) 
{
    p[0] = c1;
    p[1] = c2;
    center = (c1 + c2) * 0.5f;

	parent = pr;

    hasChildren = false;
	numVoxels = 0;
	avgNumVoxels = 0;
}

Octree::~Octree()
{
	if (hasChildren) 
	{
        DestroyChildren();
    }
}

void Octree::Add(Vector3* voxel) 
{
	if (!hasChildren && numVoxels > MAX_VOXELS_PER_OCTREE) 
	{
        HaveChildren();
    }
            
    if (hasChildren) 
	{
        AddEntry(voxel);
    }
    else 
	{
        voxels[numVoxels] = voxel;
		numVoxels++;
    }
}

void Octree::AddEntry(Vector3 *voxel) 
{
    for(int x = 0; x < 2; x++) 
	{
        if (x == 0) 
		{
			if (voxel->getX() > center[0]) 
			{
                continue;
            }
        }
        else if (voxel->getX() < center[0]) 
		{
            continue;
        }
                
        for(int y = 0; y < 2; y++) 
		{
            if (y == 0) 
			{
                if (voxel->getY() > center[1]) 
				{
                    continue;
                }
            }
            else if (voxel->getY() < center[1]) 
			{
                continue;
            }
                    
            for(int z = 0; z < 2; z++)
			{
                if (z == 0) 
				{
                    if (voxel->getZ() > center[2]) 
					{
                        continue;
                    }
                }
                else if (voxel->getZ() < center[2]) 
				{
                    continue;
                }
                        
				children[x][y][z]->Add(voxel);
            }
        }
    }
}

void Octree::HaveChildren() 
{
    for(int x = 0; x < 2; x++) 
	{
        float minX;
        float maxX;
        if (x == 0) 
		{
            minX = p[0][0];
            maxX = center[0];
        }
        else 
		{
            minX = center[0];
            maxX = p[1][0];
        }
                
        for(int y = 0; y < 2; y++) 
		{
            float minY;
            float maxY;
            if (y == 0) 
			{
                minY = p[0][1];
                maxY = center[1];
            }
            else 
			{
                minY = center[1];
                maxY = p[1][1];
            }
                    
            for(int z = 0; z < 2; z++) 
			{
                float minZ;
                float maxZ;
                if (z == 0) 
				{
                    minZ = p[0][2];
                    maxZ = center[2];
                }
                else 
				{
                    minZ = center[2];
                    maxZ = p[1][2];
                }
                        
                children[x][y][z] = new Octree(Vector3(minX, minY, minZ),
                                                Vector3(maxX, maxY, maxZ), this);

			}
        }
    }
            
	for (int i = 0; i < numVoxels; i++)
	{
        Vector3* voxel = voxels[i];
        AddEntry(voxel);
    }
    numVoxels = 0;
            
    hasChildren = true;
}

long Octree::Adfs()
{
	if (!hasChildren)
	{
		avgNumVoxels = numVoxels;
		return numVoxels;
	}

	long sum = 0;

	for(int x = 0; x < 2; x++) 
	for(int y = 0; y < 2; y++) 
	for(int z = 0; z < 2; z++) 
	{
		sum += children[x][y][z]->Adfs();
	}

	avgNumVoxels = 4;//sum * (1.0f / 8.0f);

	return sum;
}

void Octree::Intersect(const Ray &r, int *pcount)
{
	if (RayBoxIntersect(r, p))
	{
		if (hasChildren)
		{
			for(int x = 0; x < 2; x++) 
			for(int y = 0; y < 2; y++) 
			for(int z = 0; z < 2; z++) 
			{
				children[x][y][z]->Intersect(r, pcount);
			}
		}
		else
		{
			(*pcount) += parent->avgNumVoxels;//numVoxels
		}
	}
}

void Octree::DestroyChildren() 
{
	for (int i = 0; i < numVoxels; i++)
	{
		Vector3* voxel = voxels[i];
		AddEntry(voxel);
	}
	//voxels.clear();
	numVoxels = 0;
            
    for(int x = 0; x < 2; x++) 
	{
        for(int y = 0; y < 2; y++) 
		{
            for(int z = 0; z < 2; z++) 
			{
                delete children[x][y][z];
            }
        }
    }
            
    hasChildren = false;
}

#include "Octree.h"
#include "IrradianceCache.h"

Octree::Octree(Vector3 c1, Vector3 c2, int d) 
{
    corner1 = c1;
    corner2 = c2;
    center = (c1 + c2) / 2;

    depth = d;
    numCacheEntries = 0;
    hasChildren = false;
}

Octree::~Octree(void)
{
	if (hasChildren) 
	{
        destroyChildren();
    }
}

void Octree::add(IrrCacheEntry* cacheEntry) 
{
    numCacheEntries++;
    if (!hasChildren && depth < MAX_OCTREE_DEPTH && numCacheEntries > MAX_BALLS_PER_OCTREE) 
	{
        haveChildren();
    }
            
    if (hasChildren) 
	{
        addEntry(cacheEntry);
    }
    else 
	{
        cacheEntries.push_back(cacheEntry);
    }
}

//Adds a ball to or removes a from one to the children of this
void Octree::addEntry(IrrCacheEntry *cacheEntry) 
{
    //Figure out in which child(ren) the ball belongs
    for(int x = 0; x < 2; x++) 
	{
        if (x == 0) 
		{
			if (cacheEntry->position.x() > center[0]) 
			{
                continue;
            }
        }
        else if (cacheEntry->position.x() < center[0]) 
		{
            continue;
        }
                
        for(int y = 0; y < 2; y++) 
		{
            if (y == 0) 
			{
                if (cacheEntry->position.y() > center[1]) 
				{
                    continue;
                }
            }
            else if (cacheEntry->position.y() < center[1]) 
			{
                continue;
            }
                    
            for(int z = 0; z < 2; z++)
			{
                if (z == 0) 
				{
                    if (cacheEntry->position.z() > center[2]) 
					{
                        continue;
                    }
                }
                else if (cacheEntry->position.z() < center[2]) 
				{
                    continue;
                }
                        
				children[x][y][z]->add(cacheEntry);
            }
        }
    }
}

//Creates children of this, and moves the balls in this to the children
void Octree::haveChildren() 
{
    for(int x = 0; x < 2; x++) 
	{
        float minX;
        float maxX;
        if (x == 0) 
		{
            minX = corner1[0];
            maxX = center[0];
        }
        else 
		{
            minX = center[0];
            maxX = corner2[0];
        }
                
        for(int y = 0; y < 2; y++) 
		{
            float minY;
            float maxY;
            if (y == 0) 
			{
                minY = corner1[1];
                maxY = center[1];
            }
            else 
			{
                minY = center[1];
                maxY = corner2[1];
            }
                    
            for(int z = 0; z < 2; z++) 
			{
                float minZ;
                float maxZ;
                if (z == 0) 
				{
                    minZ = corner1[2];
                    maxZ = center[2];
                }
                else 
				{
                    minZ = center[2];
                    maxZ = corner2[2];
                }
                        
                children[x][y][z] = new Octree(Vector3(minX, minY, minZ),
                                                Vector3(maxX, maxY, maxZ),
                                                depth + 1);

			}
        }
    }
            
    //for(set<IrrCacheEntry*>::iterator it = cacheEntries.begin(); it != cacheEntries.end(); it++) 
	for (int i = 0; i < cacheEntries.size(); i++)
	{
        IrrCacheEntry* cacheEntry = cacheEntries[i];//*it;
        addEntry(cacheEntry);
    }
    cacheEntries.clear();
            
    hasChildren = true;
}

void Octree::destroyChildren() 
{
    //Move all balls in descendants of this to the "balls" set
    //Remove all balls from "balls" and add them to the new children
	//for(set<IrrCacheEntry*>::iterator it = cacheEntries.begin(); it != cacheEntries.end(); it++) 
	for (int i = 0; i < cacheEntries.size(); i++)
	{
		IrrCacheEntry* cacheEntry = cacheEntries[i];//*it;
		addEntry(cacheEntry);
	}
	cacheEntries.clear();
            
    for(int x = 0; x < 2; x++) {
        for(int y = 0; y < 2; y++) {
            for(int z = 0; z < 2; z++) {
                delete children[x][y][z];
            }
        }
    }
            
    hasChildren = false;
}

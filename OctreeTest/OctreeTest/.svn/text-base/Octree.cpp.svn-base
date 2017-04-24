
#include "Octree.h"
#include "glut.h"

Octree::Octree(Vector3 c1, Vector3 c2, int d) 
{
    corner1 = c1;
    corner2 = c2;
    center = (c1 + c2) / 2;

    depth = d;
    hasChildren = false;
}

Octree::~Octree(void)
{
	if (hasChildren) 
	{
        destroyChildren();
    }
}

void Octree::add(Voxel* voxel) 
{
	if (!hasChildren && depth < MAX_OCTREE_DEPTH && voxels.size() > MAX_VOXELS_PER_OCTREE) 
	{
        haveChildren();
    }
            
    if (hasChildren) 
	{
        addEntry(voxel);
    }
    else 
	{
        voxels.push_back(voxel);
    }
}

void Octree::addEntry(Voxel *voxel) 
{
    for(int x = 0; x < 2; x++) 
	{
        if (x == 0) 
		{
			if (voxel->position.x() > center[0]) 
			{
                continue;
            }
        }
        else if (voxel->position.x() < center[0]) 
		{
            continue;
        }
                
        for(int y = 0; y < 2; y++) 
		{
            if (y == 0) 
			{
                if (voxel->position.y() > center[1]) 
				{
                    continue;
                }
            }
            else if (voxel->position.y() < center[1]) 
			{
                continue;
            }
                    
            for(int z = 0; z < 2; z++)
			{
                if (z == 0) 
				{
                    if (voxel->position.z() > center[2]) 
					{
                        continue;
                    }
                }
                else if (voxel->position.z() < center[2]) 
				{
                    continue;
                }
                        
				children[x][y][z]->add(voxel);
            }
        }
    }
}

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
            
    //for(set<Irrvoxel*>::iterator it = cacheEntries.begin(); it != cacheEntries.end(); it++) 
	for (int i = 0; i < voxels.size(); i++)
	{
        Voxel* voxel = voxels[i];//*it;
        addEntry(voxel);
    }
    voxels.clear();
            
    hasChildren = true;
}

Vector3 Octree::ComputeColor()
{
	if (!hasChildren)
	{
		color = Vector3(0,0,0);

		if (voxels.size() > 0)
		{
			for (int i = 0; i < voxels.size(); i++)
			{
				color += voxels[i]->color;
			}

			color /= voxels.size();
		}
	}
	else
	{
		for(int x = 0; x < 2; x++) 
		for(int y = 0; y < 2; y++) 
		for(int z = 0; z < 2; z++) 
		{
			color += children[x][y][z]->ComputeColor() * (1.0/8.0);
		}
	}

	return color;
}

void Octree::Draw(int currentDepth, Vector3 cameraPosition)
{
	float dist = (center - cameraPosition).length();
	int stopDepth = (int)(2000 / dist);

	if (currentDepth > stopDepth || !hasChildren) 
	{
		if (color.x() > 0)
		{
			glPushMatrix();

				//glTranslatef(center.x(), center.y(), center.z());

				glColor3f(color.x(), color.y(), color.z());
				//glutSolidCube(.1);//glutSolidCube(abs(corner1.x() - corner2.x()));
				glVertex3f(center.x(), center.y(), center.z());
	
			glPopMatrix();
		}

		return;
	}

	/*glPushMatrix();

		glTranslatef(center.x(), center.y(), center.z());

		glColor3f(0.1,0.1,0.1);
		glutWireCube(abs(corner1.x() - corner2.x()));
	
	glPopMatrix();*/

	/*for (int i = 0; i < voxels.size(); i++)
	{
		glPushMatrix();

			glTranslatef(voxels[i]->position.x(), voxels[i]->position.y(), voxels[i]->position.z());

			glColor3f(voxels[i]->color.x(), voxels[i]->color.y(), voxels[i]->color.z());
			glutSolidCube(1);
	
		glPopMatrix();
	}*/

	if (hasChildren)
	{
		for(int x = 0; x < 2; x++) 
		for(int y = 0; y < 2; y++) 
		for(int z = 0; z < 2; z++) 
		{
			children[x][y][z]->Draw(currentDepth+1, cameraPosition);
		}
	}
}

void Octree::destroyChildren() 
{
    //Move all balls in descendants of this to the "balls" set
    //Remove all balls from "balls" and add them to the new children
	//for(set<Irrvoxel*>::iterator it = cacheEntries.begin(); it != cacheEntries.end(); it++) 
	for (int i = 0; i < voxels.size(); i++)
	{
		Voxel* voxel = voxels[i];//*it;
		addEntry(voxel);
	}
	voxels.clear();
            
    for(int x = 0; x < 2; x++) {
        for(int y = 0; y < 2; y++) {
            for(int z = 0; z < 2; z++) {
                delete children[x][y][z];
            }
        }
    }
            
    hasChildren = false;
}

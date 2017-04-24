#include "IrradianceCache.h"


IrradianceCache::IrradianceCache(const int _maxCacheEntries, Vector3 &min, Vector3 &max)
{
	maxCacheEntries = _maxCacheEntries;

	numCacheEntries = 0;

	minWeight = 10.0f;

	octree = new Octree(min, max, 0);

	InitializeCriticalSection(&CriticalSection);
}

IrradianceCache::~IrradianceCache()
{
	DeleteCriticalSection(&CriticalSection);
}

void IrradianceCache::AddCacheEntry(IrrCacheEntry *cacheEntry)
{
	EnterCriticalSection(&CriticalSection); 

	if ( numCacheEntries < maxCacheEntries-1)
	{
		octree->add(cacheEntry);
		numCacheEntries++;
	}
	else
	{
		cout << "\n!!! Irradiance Cache is already full!\n";
	}

	LeaveCriticalSection(&CriticalSection);
}   

bool ContainsPoint(Vector3 p, Vector3 min, Vector3 max)
{
	if (p.x() < min.x()) return false;
	if (p.y() < min.y()) return false;
	if (p.z() < min.z()) return false;

	if (p.x() > max.x()) return false;
	if (p.y() > max.y()) return false;
	if (p.z() > max.z()) return false;

	return true;
}

inline float sqr(float x)
{
	x = x * x;
	return x;
}

bool SphereBoxOverlap(const Vector3 &pMin, const Vector3 &pMax, const Vector3 &c, const float r) 
{
	float dmin = 0;
	
	for (int i = 0; i < 3; i++)
	{
		if      ( c[i] < pMin[i] ) dmin += sqr( c[i] - pMin[i] );
		else if ( c[i] > pMax[i] ) dmin += sqr( c[i] - pMax[i] );
	}

	return dmin <= r*r;
}

void IrradianceCache::DrawPoints(Octree *node, Image *image)
{
	for (int i = 0; i < node->cacheEntries.size(); i++)
	{
		IrrCacheEntry *cacheEntry = node->cacheEntries[i];//*it;

		Vector2 screenPos = cacheEntry->screenPosition;

		image->setPixel(screenPos.x(), screenPos.y(), rgb(0,10000,0));
	}

	if (node->hasChildren)
	{
		for(int x = 0; x < 2; x++) 
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++) 
				{
					DrawPoints(node->children[x][y][z], image);
				}
			}
		}
	}
}

void IrradianceCache::GetCacheValue(int &entriesUsed, rgb &sum_weightsByValues, float &sum_weights, const Octree *node, const Vector3 &normal, const Vector3 &position)
{
	EnterCriticalSection(&CriticalSection); 

	for (int i = 0; i < node->cacheEntries.size(); i++)
	{
		if ( dot(normal, node->cacheEntries[i]->normal) < 0.9f ) // Is the cache entry facing the correct direction?
		{
			continue;
		}

		const Vector3 delta = node->cacheEntries[i]->position - position;
		const float dist = delta.length();

		if ( dist > node->cacheEntries[i]->radius ) // Is sample point within the validity radius (world space)?
		{
			continue;
		}
			
		const Vector3 navg = node->cacheEntries[i]->normal + normal;

		if ( dot(position - node->cacheEntries[i]->position, navg * 0.5f) < -.01f )
		{
			continue;
		}

		float d = dot(normal, node->cacheEntries[i]->normal);
		if (d > 0.999999) d = 0.999999;
		float root = sqrtf(1.0f - d);
		float weight = 1.0f / ( (dist / node->cacheEntries[i]->radius) + root);

		if (weight < minWeight)
		{
			continue;
		}

		rgb value = node->cacheEntries[i]->value;

		for (int i = 0; i < 3; i++)
		{
			if (value[i] < 0) value[i] = 0.0f;
		}

		sum_weightsByValues += weight * value;

		sum_weights += weight;

		entriesUsed++;
	}

	if (node->hasChildren)
	{
		for(int x = 0; x < 2; x++) 
		{
			for(int y = 0; y < 2; y++)
			{
				for(int z = 0; z < 2; z++) 
				{
					Vector3 delta = node->corner1 - node->corner2;
					
					if (SphereBoxOverlap(node->children[x][y][z]->corner1, node->children[x][y][z]->corner2, position, delta.length() * 0.5f))
					{
						GetCacheValue(entriesUsed, sum_weightsByValues, sum_weights, node->children[x][y][z], normal, position);
					}
				}
			}
		}
	}

	LeaveCriticalSection(&CriticalSection);
}

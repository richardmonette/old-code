#pragma once

#include <Windows.h>

#include "rgb.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Octree.h"
#include "Image.h"

struct IrrCacheEntry
{
	Vector3 position; // position in space
	Vector3 normal;   // normal direction
	int lvl;          // recursion level of parent ray
	float weight;     // weight of parent ray
	float radius;     // vailidity radius
	rgb value;        // computed ambient value

	Vector2 screenPosition;
};

class IrradianceCache
{
public:
	IrradianceCache(const int _maxCacheEntries, Vector3 &min, Vector3 &max);
	~IrradianceCache();

	void AddCacheEntry(IrrCacheEntry *cacheEntry);

	void GetCacheValue(int &entriesUsed, rgb &sum_weightsByValues, float &sum_weights, const Octree *node, const Vector3 &normal, const Vector3 &position);

	void DrawPoints(Octree *node, Image *image);

	Octree *octree;

	int numCacheEntries;
	int maxCacheEntries;

	float minWeight;

	CRITICAL_SECTION CriticalSection; 
};


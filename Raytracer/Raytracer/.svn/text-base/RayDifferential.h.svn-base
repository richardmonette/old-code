#pragma once

#include "Ray.h"

class RayDifferential :
	public Ray
{
public:
	RayDifferential() { hasDifferentials = false; };
	RayDifferential( const Point &org, const Vector & dir) : Ray( org, dir ) { hasDifferentials = false; };
	explicit RayDifferential(const Ray &ray) : Ray(ray) { hasDifferentials = false; };
	~RayDifferential();

	bool hasDifferentials;
	Ray rx, ry;
};

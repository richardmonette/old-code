#pragma once

#include "GlobalDefine.h"
#include "Point.h"

class Ray
{
public:
	Ray() : mint(RAY_EPSILON), maxt(INFINITY), time(0.f) {};
	Ray(const Point &origin, const Vector &direction, float start = RAY_EPSILON, float end = INFINITY, float t = 0.f) 
		: o(origin), d(direction), mint(start), maxt(end), time(t) {};
	Ray(const Point &p1, const Point &p2, float start = RAY_EPSILON, float end = INFINITY)
	{
		o = p1;
		d = p1-p2;
	}

	~Ray();

	Point operator()(float t) const { return o + d * t; };

	Point o;
	Vector d;

	mutable float mint, maxt;
	float time;
};

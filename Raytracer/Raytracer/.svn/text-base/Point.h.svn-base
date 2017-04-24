#pragma once

#include "Vector.h"

class Point
{
public:
	Point(float _x=0, float _y=0, float _z=0) 
		:x(_x), y(_y), z(_z) {};
	~Point();

	Point operator+(const Vector &v) const
	{
		return Point(x + v.x, y + v.y, z + v.z);
	}

	Point &operator+=(const Vector &v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector operator-(const Point &p) const 
	{
		return Vector(x - p.x, y - p.y, z - p.z);
	}

	Point operator-(const Vector &v) const 
	{
		return Point(x - v.x, y - v.y, z - v.z);
	}

	Point &operator-=(const Vector &v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	inline float Distance(const Point &p1, const Point &p2)
	{
		return (p1 - p2).Length();
	}

	inline float DistanceSquared(const Point &p1, const Point &p2)
	{
		return (p1 - p2).LengthSquared();
	}

	float x,y,z;
};

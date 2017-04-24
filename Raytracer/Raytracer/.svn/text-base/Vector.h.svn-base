#pragma once

#include "math.h"
#include "GlobalDefine.h"

class Normal;

class Vector
{
public:
	Vector(float _x=0, float _y=0, float _z=0) 
		: x(_x), y(_y), z(_z) {};

	~Vector(void);

	Vector operator+(const Vector &v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector &operator+=(const Vector &v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector operator*(float f) const
	{
		return Vector(f*x, f*y, f*z);
	}

	Vector &operator*=(float f)
	{
		x *= f; y *= f; z *= f;
		return *this;
	}

	explicit Vector(const Normal &n);

	/*inline Vector operator*(float f, const Vector &v)
	{
		return v * f;
	}*/

	Vector operator/(float f) const 
	{
		Assert(f!=0);
		float inv = 1.f / f;
		return Vector(x * inv, y * inv, z * inv);
	}

	Vector &operator/=(float f)
	{
		Assert(f!=0);
		float inv = 1.f / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	Vector operator-() const 
	{
		return Vector(-x, -y, -z);
	}

	float operator[](int i) const
	{
		Assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float &operator[](int i)
	{
		Assert(i >= 0 && i <= 2);
		return (&x)[i];
	}

	float LengthSquared() const { return x*x + y*y + z*z; }
	float Length() const { return sqrtf(LengthSquared()); }

	float x,y,z;
};

class Normal
{
public:
	
	Normal::Normal();
	explicit Normal(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

	float x,y,z;
};

inline Vector::Vector(const Normal &n)
: x(n.x), y(n.y), z(n.z)
{
}

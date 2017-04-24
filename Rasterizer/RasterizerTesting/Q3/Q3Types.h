#pragma once

#include "math.h"

struct Q3Vector3
{
public:

	Q3Vector3() {}

	Q3Vector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	Q3Vector3 operator+(Q3Vector3 vVector)
	{
		return Q3Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	Q3Vector3 operator-(Q3Vector3 vVector)
	{
		return Q3Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	Q3Vector3& operator+=(const Q3Vector3& vec) 
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	Q3Vector3 operator*(float num)
	{
		return Q3Vector3(x * num, y * num, z * num);
	}

	Q3Vector3 operator/(float num)
	{
		return Q3Vector3(x / num, y / num, z / num);
	}

	Q3Vector3 Cross(Q3Vector3 vVector2)
	{
		Q3Vector3 vNormal;	

		vNormal.x = ((y * vVector2.z) - (z * vVector2.y));
		vNormal.y = ((z * vVector2.x) - (x * vVector2.z));
		vNormal.z = ((x * vVector2.y) - (y * vVector2.x));

		return vNormal;										 
	}

	float Magnitude()
	{
		return (float)sqrt( (x * x) + (y * y) + (z * z) );
	}

	void Normalize()
	{
		float magnitude = (float)sqrt( (x * x) + (y * y) + (z * z) );				

		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}

	float x, y, z;						
};

class Q3Vector2 
{
public:

	Q3Vector2() {}

	Q3Vector2(float X, float Y) 
	{ 
		x = X; y = Y;
	}

	Q3Vector2 operator+(Q3Vector2 vVector)
	{
		return Q3Vector2(vVector.x + x, vVector.y + y);
	}

	Q3Vector2 operator-(Q3Vector2 vVector)
	{
		return Q3Vector2(x - vVector.x, y - vVector.y);
	}

	Q3Vector2 operator*(float num)
	{
		return Q3Vector2(x * num, y * num);
	}

	Q3Vector2 operator/(float num)
	{
		return Q3Vector2(x / num, y / num);
	}

	float x, y;
};

struct tBSPVertex
{
	Q3Vector3 vPosition;
	Q3Vector2 vTextureCoord;
	Q3Vector2 vLightmapCoord;
	Q3Vector3 vNormal;
	char color[4];
};

struct tBSPLightmap
{
	char imageBits[128][128][3];
};
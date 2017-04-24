
#pragma once

#include "Mat4x4.h"
#include "math.h"

struct Vector3
{
public:
	
	Vector3() {}

	Vector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	Vector3 operator+(Vector3 vVector)
	{
		return Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	Vector3 operator-(Vector3 vVector)
	{
		return Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	Vector3& operator+=(const Vector3& vec) 
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}
	
	Vector3 operator*(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}

	// Vector by 4x4 matrix

		Vector3 operator*(const Mat4x4& mat) const {
			return Vector3(x * mat._11 + y * mat._21 + z * mat._31,
						   x * mat._12 + y * mat._22 + z * mat._32,
						   x * mat._13 + y * mat._23 + z * mat._33);
		}

	// Vector by 4x4 matrix

	Vector3 operator/(float num)
	{
		return Vector3(x / num, y / num, z / num);
	}

	Vector3 Cross(Vector3 vVector2)
	{
		Vector3 vNormal;	

		vNormal.x = ((y * vVector2.z) - (z * vVector2.y));
		vNormal.y = ((z * vVector2.x) - (x * vVector2.z));
		vNormal.z = ((x * vVector2.y) - (y * vVector2.x));

		return vNormal;										 
	}

	void Zero()
	{
		x = 0;
		y = 0;
		z = 0;
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
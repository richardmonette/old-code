#pragma once

#include "Math3D.h"

class Plane  
{

public:

	float a, b, c, distance;

	Plane::Plane();
	Plane::~Plane();

	/*void Plane::Set3Points( Vec3 &v1,  Vec3 &v2,  Vec3 &v3);
	void SetCoefficients(float a, float b, float c, float d);
	*/
	void Normalize();

	float Distance(Vec4 *p);
	float Distance(Vec4 &p);
	float Distance(Vec3 &p);

	int intersect( Vec4 &P0, Vec4 &P1, Vec4 *out ) 
	{ 
		Vec4 origin = P0;
		Vec4 direction = (P1 - P0);
		//normalizeSelfFAST(direction);
		Vec4 normal(a, b, c, 1.0f);

		float d = dot(normal, direction);

		if (d != 0)
		{
			float dist = -( dot(normal, origin)  + distance) / d;
			if (dist > 0)
			{
				*out = P0 + dist * direction; 
				return 1;
			}
		}

		return 0;
	}

};

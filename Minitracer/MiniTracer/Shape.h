
#pragma once

#include "Global.h"

class Shape
{
public:

	Vec emission, color;
	Refl_t reflectionType;      // reflection type (DIFFuse, SPECular, REFRactive)

	virtual int intersect(const Ray& rayRef, double& distRef) const = 0;
	virtual Vec getnormal(Vec &x) const = 0;
};

class Plane : public Shape
{
public:
	Plane(Vec n_, float d_, Vec e_, Vec c_, Refl_t refl_):
		normal(n_), distance(d_) 
		{
			reflectionType = refl_;
			emission = e_;
			color = c_;
		}

	Vec getnormal(Vec &x) const
	{
		return normal;
	}

	int intersect(const Ray& rayRef, double& distRef) const 
	{ 
		float d = dot(normal, rayRef.direction);

		if (d != 0)
		{
			float dist = -( dot(normal, rayRef.origin)  + distance) / d;
			if (dist > 0)
			{
				if (dist < distRef) 
				{
					distRef = dist;
					return HIT;
				}
			}
		}

		return MISS;
	}

	Vec normal;
	float distance;
};

class Sphere : public Shape
{	
public:
	Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
	rad(rad_), p(p_) 
	{
		reflectionType = refl_;
		emission = e_;
		color = c_;
	}

	Vec getnormal(Vec &x) const
	{
		return (x-p).norm();
	}
	
	int intersect(const Ray& rayRef, double& distRef) const 
	{ 
		Vec op = p-rayRef.origin;
		double b = dot(op, rayRef.direction);
		double det = b * b - dot(op, op) + rad*rad;
		
		int retval = MISS;
		if (det > 0)
		{
			det = sqrtf( det );
			float i1 = b - det;
			float i2 = b + det;
			if (i2 > 0)
			{
				if (i1 < 0) 
				{
					if (i2 < distRef) 
					{
						distRef = i2;
						retval = INPRIM;
					}
				}
				else
				{
					if (i1 < distRef)
					{
						distRef = i1;
						retval = HIT;
					}
				}
			}
		}
		return retval;
	}

	double rad;       // radius
	Vec p;
};

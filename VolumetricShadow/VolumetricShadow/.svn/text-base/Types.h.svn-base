
#pragma once

#include <math.h>
#include <vectormath_aos.h>
#include "ray.h"

using namespace Vectormath::Aos;

/*static float RaySphereIntersection(const Ray &r, const Vector3 &p) 
{ 
	// returns distance, 0 if nohit 

	const float rad = 0.15f;

	Vector3 op = p - r.origin; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 
	
	float b = dot(op, r.direction);
	float det = b * b - dot(op, op) + rad * rad;
	
	if (det < 0) 
	{
		return 0;
	}
	else
	{
		det = sqrt(det);
	}

	float t;
	float eps = 1e-4;

	return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0); 
}*/

static bool RaySphereIntersection(const Ray &r, const Vector3 &p) 
{ 
	const float rad = 0.15f;

	Vector3 l = p - r.origin;
	float s = dot(l, r.direction);
	float l2 = dot(l, l);
	float r2 = rad*rad;
	if (s < 0 && l2 > r2) return false;
	float m2 = l2 - s*s;
	if (m2 > r2) return false;
	
	return true;	
} 

#define t0 0.1
#define t1 999999

static bool RayBoxIntersect(const Ray &r, const Vector3 parameters[2]) 
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (parameters[r.sign[0]].getX() - r.origin.getX()) * r.inv_direction.getX();
	tmax = (parameters[1-r.sign[0]].getX() - r.origin.getX()) * r.inv_direction.getX();
	tymin = (parameters[r.sign[1]].getY() - r.origin.getY()) * r.inv_direction.getY();
	tymax = (parameters[1-r.sign[1]].getY() - r.origin.getY()) * r.inv_direction.getY();

	if ( (tmin > tymax) || (tymin > tmax) ) 
		return false;
	
	if (tymin > tmin)
		tmin = tymin;
	
	if (tymax < tmax)
		tmax = tymax;
	
	tzmin = (parameters[r.sign[2]].getZ() - r.origin.getZ()) * r.inv_direction.getZ();
	tzmax = (parameters[1-r.sign[2]].getZ() - r.origin.getZ()) * r.inv_direction.getZ();
	
	if ( (tmin > tzmax) || (tzmin > tmax) ) 
		return false;
	
	if (tzmin > tmin)
		tmin = tzmin;
	
	if (tzmax < tmax)
		tmax = tzmax;
	
	return ( (tmin < t1) && (tmax > t0) );
}
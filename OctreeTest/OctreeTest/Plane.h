
#ifndef _PLANE_
#define _PLANE_

#include "Vector3.h"

class Plane  
{

public:

	Vector3 normal, point;
	float d;


	Plane::Plane( Vector3 &v1,  Vector3 &v2,  Vector3 &v3);
	Plane::Plane(void);
	Plane::~Plane();

	void set3Points( Vector3 &v1,  Vector3 &v2,  Vector3 &v3);
	void setNormalAndPoint(Vector3 &normal, Vector3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(Vector3 &p);

};


#endif
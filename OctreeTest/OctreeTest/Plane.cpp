
#include "Plane.h"
#include <stdio.h>

Plane::Plane( Vector3 &v1,  Vector3 &v2,  Vector3 &v3) 
{
	set3Points(v1,v2,v3);
}

Plane::Plane() {}

Plane::~Plane() {}

void Plane::set3Points( Vector3 &v1,  Vector3 &v2,  Vector3 &v3) 
{
	Vector3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = cross(aux2, aux1);

	normal.makeUnitVector();
	point = v2;
	d = -(dot(normal, point));
}

void Plane::setNormalAndPoint(Vector3 &_normal, Vector3 &point) 
{
	this->normal - _normal;
	this->normal.makeUnitVector();
	d = -(dot(this->normal, point));
}

void Plane::setCoefficients(float a, float b, float c, float d) 
{
	// set the normal vector
	normal = Vector3(a,b,c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal = Vector3(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;
}

float Plane::distance(Vector3 &p) 
{
	return (d + dot(normal, p));
}

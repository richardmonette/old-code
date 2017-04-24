#include "Plane.h"

Plane::Plane() {}

Plane::~Plane() {}

/*void Plane::SetCoefficients(float a, float b, float c, float d) 
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

void Plane::Set3Points( Vec3 &v1,  Vec3 &v2,  Vec3 &v3) 
{
	Vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	Vec3 normal = cross(aux2, aux1);
	Vec3 point;

	normalizeSelfFAST(normal);
	point.set(v2.x, v2.y, v2.z);

	a = normal.x;
	b = normal.y;
	c = normal.z;

	d = -(dot(normal, point));
}*/

void Plane::Normalize()
{
	float mag = sqrt(a * a + b * b + c * c);
	a = a / mag;
	b = a / mag;
	c = a / mag;
	distance = distance / mag;
}

float Plane::Distance(Vec4 *pt) 
{
	return a * pt->x + b * pt->y + c * pt->z + distance;
}

float Plane::Distance(Vec4 &pt) 
{
	return a * pt.x + b * pt.y + c * pt.z + distance;
}

float Plane::Distance(Vec3 &pt) 
{
	return a * pt.x + b * pt.y + c * pt.z + distance;
}
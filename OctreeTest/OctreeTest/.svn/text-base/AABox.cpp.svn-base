
#include "AABox.h"
#include "Vector3.h"

AABox::AABox( Vector3 &corner,  float x, float y, float z) 
{
	setBox(corner,x,y,z);
}

AABox::AABox(void) 
{
	corner.setX(0);
	corner.setY(0);
	corner.setZ(0);

	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
}

AABox::~AABox() {}

void AABox::setBox( Vector3 &_corner,  float x, float y, float z) 
{
	this->corner = _corner;

	if (x < 0.0) {
		x = -x;
		this->corner.e[0] -= x;
	}
	if (y < 0.0) {
		y = -y;
		this->corner.e[1] -= y;
	}
	if (z < 0.0) {
		z = -z;
		this->corner.e[2] -= z;
	}
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 AABox::getVertexP(Vector3 &normal) 
{
	Vector3 res = corner;

	if (normal.x() > 0)
		res.e[0] += x;

	if (normal.y() > 0)
		res.e[1] += y;

	if (normal.z() > 0)
		res.e[2] += z;

	return(res);
}

Vector3 AABox::getVertexN(Vector3 &normal)
{
	Vector3 res = corner;

	if (normal.x() < 0)
		res.e[0] += x;

	if (normal.y() < 0)
		res.e[1] += y;

	if (normal.z() < 0)
		res.e[2] += z;

	return(res);
}

	



#ifndef _AABOX_
#define _AABOX_

#include "Vector3.h"

class AABox 
{

public:

	Vector3 corner;
	float x,y,z;


	AABox::AABox( Vector3 &corner, float x, float y, float z);
	AABox::AABox(void);
	AABox::~AABox();

	void AABox::setBox( Vector3 &corner, float x, float y, float z);

	// for use in frustum computations
	Vector3 AABox::getVertexP(Vector3 &normal);
	Vector3 AABox::getVertexN(Vector3 &normal);

};

#endif
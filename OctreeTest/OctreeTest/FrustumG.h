
#ifndef _FRUSTUMG_
#define _FRUSTUMG_

#include "Vector3.h"
#include "Plane.h"
#include "AABox.h"

class FrustumG 
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

	static enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];

	Vector3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD);
	void FrustumG::setCamDef(Vector3 &p, Vector3 &l, Vector3 &u);
	int FrustumG::pointInFrustum(Vector3 &p);
	int FrustumG::sphereInFrustum(Vector3 &p, float raio);
	int FrustumG::boxInFrustum(AABox &b);
};


#endif
#pragma once

#include "Shape.h"
#include "Vector.h"
#include "Geometry.h"
#include "Point.h"

class DifferentialGeometry
{
public:
	DifferentialGeometry() { u = v = 0.; shape = NULL; }

	DifferentialGeometry(const Point &P, const Vector &DPDU, const Vector &DPDV, const Vector &DNDU, const Vector &DNDV, float uu, float vv, const Shape *sh)
		: p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV)
	{
		nn = Normal(Normalize(Cross(dpdu, dpdv)));
		u = uu;
		v = vv;
		shape = sh;
	}

	~DifferentialGeometry();

	Point p;
	Normal nn;
	float u, v;
	const Shape *shape;

	Vector dpdu, dpdv;
	Vector dndu, dndv;
};


#pragma once

#include "Scene.h"
#include "Spectrum.h"
#include "Point.h"
#include "Vector.h"

struct VisibilityTester
{
	void SetSegment(const Point &p1, const Point &p2)
	{
		r = Ray(p1, p2, RAY_EPSILON, 1.f - RAY_EPSILON);
	}

	void SetRay(const Point &p, const Vector &w)
	{
		r = Ray(p, w, RAY_EPSILON);
	}

	bool Unoccluded(const Scene *scene)
	{
		return !scene->IntersectP(r);
	}

	Spectrum Transmittance(const Scene *scene) const
	{
		return scene->Transmittance(r);
	}

	Ray r;
};
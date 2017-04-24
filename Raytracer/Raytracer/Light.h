#pragma once

#include "Transform.h"
#include "Spectrum.h"
#include "Scene.h"
#include "Ray.h"
#include "VisibilityTester.h"
#include "Point.h"
#include "Vector.h"

class Light
{
public:
	Light(const Transform &l2w, int ns = 1)
		: nSamples(max(1, ns)), LightToWorld(l2w), WorldToLight(l2w.GetInverse())
	{
	}
	~Light();

	virtual Spectrum Sample_L(const Point &p, Vector *wi, VisibilityTester *vis) const = 0;
	virtual Spectrum Power(const Scene *scene) const = 0;
	virtual bool IsDeltaLight() const = 0;

	const int nSamples;

protected:

	const Transform LightToWorld, WorldToLight;

};

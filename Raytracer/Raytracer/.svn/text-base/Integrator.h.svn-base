#pragma once

#include "Spectrum.h"

class Scene;
class RayDifferential;
class Sample;

class Integrator
{
public:

	Integrator();
	~Integrator();

	virtual Spectrum Li(const Scene *scene, const RayDifferential &ray, const Sample *sample, float *alpha) const = 0;
	virtual void Preprocess(const Scene *scene) const = 0;
	virtual void RequestSamples(Sample *sample, const Scene *scene) const = 0;
};

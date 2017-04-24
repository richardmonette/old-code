#pragma once

#include "SurfaceIntegrator.h"

class Scene;

enum LightStrategy { SAMPLE_ALL_UNIFORM, SAMPLE_ONE_UNIFORM };

class DirectLighting :
	public SurfaceIntegrator
{
public:
	DirectLighting(LightStrategy lstrat);
	~DirectLighting();

	void RequestSamples(Sample *sample, const Scene *scene);

private:

	int *lightSampleOffset, lightNumOffset;
	int * bsdfSampleOffset, *bsdfComponentOffset;

	LightStrategy strategy;
};

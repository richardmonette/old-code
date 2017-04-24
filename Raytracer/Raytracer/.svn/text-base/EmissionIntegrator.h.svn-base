#pragma once

#include "VolumeIntegrator.h"

class EmissionIntegrator : public VolumeIntegrator
{
public:
	EmissionIntegrator(float ss);
	~EmissionIntegrator(void);

	void RequestSamples(Sample *sample, const Scene *scene);

	Spectrum Transmittance(const Scene *scene, const Ray &Ray, const Sample *sample, float *alpha);
	Spectrum Li(const Scene *scene, const RayDifferential &ray, const Sample *sample, float *alpha);

private:

	float stepSize;
	int tauSampleOffset, scatterSampleOffset;

};

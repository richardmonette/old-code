
#pragma once

#include "Scene.h"
#include "Ray.h"
#include "Sample.h"
#include "Integrator.h"

class VolumeIntegrator : public Integrator
{
public:

	VolumeIntegrator();
	~VolumeIntegrator();

	virtual Spectrum Transmittance(const Scene *scene, const Ray &Ray, const Sample *sample, float *alpha) const = 0;
};
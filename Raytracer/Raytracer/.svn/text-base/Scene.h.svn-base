#pragma once

#include "BBox.h"

#include <vector>

class Intersection;
class RayDifferential;
class Sample;
class Ray;
class Primitive;
class Light;
class Camera;
class VolumeRegion;
class SurfaceIntegrator;
class VolumeIntegrator;
class Sampler;
class Spectrum;

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void Render();

	bool Intersect(const Ray &ray, Intersection *isect) const;

	bool IntersectP(const Ray &ray) const;

	const BBox &Scene::WorldBound() const;

	Spectrum Li(const RayDifferential &ray, const Sample *sample, float *alpha) const;

	Spectrum Transmittance(const Ray &ray) const;

	BBox bound;

	Primitive *aggregate;

	vector<Light*> lights;

	Camera *camera;
	VolumeRegion *volumeRegion;

	SurfaceIntegrator *surfaceIntegrator;
	VolumeIntegrator *volumeIntegrator;

	Sampler *sampler;
};

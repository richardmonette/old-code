#include "Scene.h"
#include "Primitive.h"
#include "Spectrum.h"
#include "VolumeIntegrator.h"
#include "SurfaceIntegrator.h"
#include "GlobalDefine.h"
#include "Sampler.h"
#include "Camera.h"
#include "Film.h"
#include "RayDifferential.h"

Scene::Scene()
{
	bound = aggregate->WorldBound();
}

Scene::~Scene()
{
}

bool Scene::Intersect(const Ray &ray, Intersection *isect) const
{
	return aggregate->Intersect(ray, isect);
}

bool Scene::IntersectP(const Ray &ray) const
{
	return aggregate->IntersectP(ray);
}

const BBox &Scene::WorldBound() const
{
	return bound;
}

Spectrum Scene::Li(const RayDifferential &ray, const Sample *sample, float *alpha) const
{
	Spectrum Lo = surfaceIntegrator->Li(this, ray, sample, alpha);
	Spectrum T = volumeIntegrator->Transmittance(this, ray, sample, alpha);
	Spectrum Lv = volumeIntegrator->Li(this, ray, sample, alpha);
	return T * Lo + Lv;
}

Spectrum Scene::Transmittance(const Ray &ray) const
{
	return volumeIntegrator->Transmittance(this, ray, NULL, NULL);
}

void Scene::Render()
{
	Sample *sample = new Sample(surfaceIntegrator, volumeIntegrator, this);
	surfaceIntegrator->Preprocess(this);
	volumeIntegrator->Preprocess(this);

	while (sampler->GetNextSample(sample))
	{
		RayDifferential ray;
		float rayWeight = camera->GenerateRay(*sample, &ray);
		++(sample->imageX);
		camera->GenerateRay(*sample, &ray.rx);
		--(sample->imageX);
		++(sample->imageY);
		camera->GenerateRay(*sample, &ray.ry);
		ray.hasDifferentials = true;
		--(sample->imageY);

		float alpha;
		Spectrum Ls = 0.f;
		if (rayWeight > 0.f)
		{
			Ls = rayWeight * Li(ray, sample, &alpha);
		}

		camera->film->AddSample(*sample, ray, Ls, alpha);

		BSDF::FreeAll();
	}

	delete sample;

	camera->film->WriteImage();
}

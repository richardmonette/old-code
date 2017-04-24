#include "EmissionIntegrator.h"
#include "Spectrum.h"
#include "VolumeRegion.h"
#include "RayDifferential.h"

EmissionIntegrator::EmissionIntegrator(float ss)
{
	stepSize = ss;
}

EmissionIntegrator::~EmissionIntegrator(void)
{
}

void EmissionIntegrator::RequestSamples(Sample *sample, const Scene *scene)
{
	tauSampleOffset = sample->Add1D(1);
	scatterSampleOffset = sample->Add1D(1);
}

Spectrum EmissionIntegrator::Transmittance(const Scene *scene, const Ray &ray, const Sample *sample, float *alpha)
{
	if (!scene->volumeRegion) return Spectrum(1.f);
	float step = sample ? stepSize : 4.f * stepSize;
	float offset = sample ? sample->oneD[tauSampleOffset][0] : RandomFloat();
	Spectrum tau = scene->volumeRegion->Tau(ray, step, offset);
	return exp(-tau);
}

Spectrum EmissionIntegrator::Li(const Scene *scene, const RayDifferential &ray, const Sample *sample, float *alpha)
{
	VolumeRegion *vr = scene->volumeRegion;
	float t0, t1;
	if (!vr || !vr->IntersectP(ray, &t0, &t1)) return 0.0f;
	Spectrum Lv(0.0f);
	int N = Ceil2Int((t1-t0) / stepSize);
	float step = (t1 - t0) / N;
	Spectrum Tr(1.f);
	Point p = ray(t0), pPrev;
	Vector w = -ray.d;
	
	if (sample)
		t0 += sample->oneD[scatterSampleOffset][0] * step;
	else
		t0 += RandomFloat() * step;

	for (int i = 0; i < N; ++i, t0 += step)
	{
		pPrev = p;
		p = ray(t0);
		Spectrum stepTau = vr->Tau(Ray(pPrev, p-pPrev, 0, 1), .5f * stepSize, RandomFloat());
		Tr *= Exp(-stepTau);

		if (Tr.y() < 1e-13)
		{
			const float continueProb = .5f;
			if (RandomFloat() > continueProb) break;
			Tr /= continueProb;
		}

		Lv += Tr * vr->Lve(p,w);
	}
}
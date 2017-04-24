#include "DirectLighting.h"
#include "Scene.h"
#include "Sample.h"
#include "Sampler.h"
#include "Scene.h"
#include "Light.h"

DirectLighting::DirectLighting(LightStrategy lstrat)
{
	strategy = lstrat;
}

DirectLighting::~DirectLighting()
{
}

void DirectLighting::RequestSamples(Sample *sample, const Scene *scene)
{
	if (strategy == SAMPLE_ALL_UNIFORM)
	{
		unsigned int nLights = scene->lights.size();
		lightSampleOffset = new int[nLights];
		bsdfSampleOffset = new int [nLights];
		bsdfComponentOffset = new int[nLights];
		for (unsigned int i = 0; i < nLights; ++i)
		{
			const Light *light = scene->lights[i];
			int lightSamples = scene->sampler->RoundSize(light->nSamples);
			lightSampleOffset[i] = sample->Add2D(lightSamples);
			bsdfSampleOffset[i] = sample->Add2D(lightSamples);
			bsdfComponentOffset[i] = sample->Add1D(lightSamples);
		}
		lightNumOffset = -1;
	}
	else if (strategy == SAMPLE_ONE_UNIFORM)
	{
		lightSampleOffset = new int [1];
		lightSampleOffset[0] = sample->Add2D(1);
		lightNumOffset = sample->Add1D(1);
		bsdfSampleOffset = new int[1];
		bsdfSampleOffset[0] = sample->Add2D(1);
		bsdfComponentOffset = new int[1];
		bsdfComponentOffset[0] = sample->Add2D(1);
	}
}

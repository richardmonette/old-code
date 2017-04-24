
#pragma once

#include "Sample.h"
#include "Ray.h"
#include "Spectrum.h"

class Film 
{
public:

	Film(int xres, int yres) : xResolution(xres), yResolution(yres)
	{

	}

	virtual void AddSample(const Sample &sample, const Ray &ray, const Spectrum &L, float alpha) = 0;

	virtual void WriteImage() = 0;
	virtual void GetSampleExtent(int *xstart, int *xend, int *ystart, int *yend) const = 0;

	const int xResolution, yResolution;
};
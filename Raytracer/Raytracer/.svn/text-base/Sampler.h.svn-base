
#pragma once

class Sample;

class Sampler
{

public:

	Sampler::Sampler(int xstart, int xend, int ystart, int yend, int spp)
	{
		xPixelStart = xstart;
		xPixelEnd = xend;
		yPixelStart = ystart;
		yPixelEnd = yend;
		samplesPerPixel = spp;
	}

	virtual bool GetNextSample(Sample *sample) = 0;

	virtual int RoundSize(int size) const = 0;

	int TotalSamples() const 
	{
		return samplesPerPixel * 
				(xPixelEnd - xPixelStart) * 
				(yPixelEnd - yPixelStart);
	}

	int xPixelStart, xPixelEnd, yPixelStart, yPixelEnd;
	int samplesPerPixel;

};
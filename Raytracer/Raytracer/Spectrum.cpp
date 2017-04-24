#include "Spectrum.h"

Spectrum::Spectrum(float v)
{
	for (int i = 0; i < COLOR_SAMPLES; ++i)
	{
		c[i] = v;
	}
}

Spectrum::Spectrum(float cs[COLOR_SAMPLES])
{
for (int i = 0; i < COLOR_SAMPLES; ++i)
	{
		c[i] = cs[i];
	}
}

Spectrum::~Spectrum(void)
{
}

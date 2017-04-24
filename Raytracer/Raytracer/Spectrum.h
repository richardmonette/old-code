#pragma once

#include "math.h"

#define COLOR_SAMPLES 3

class Spectrum
{
public:

	Spectrum(float v = 0.f);
	Spectrum(float cs[COLOR_SAMPLES]);
	~Spectrum();

	Spectrum &operator+=(const Spectrum &s2)
	{
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			c[i] += s2.c[i];
		}
		return *this;
	}

	Spectrum operator+(const Spectrum &s2)
	{
		Spectrum ret = *this;
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			ret.c[i] += s2.c[i];
		}
		return ret;
	}

	void AddWeighted(float w, const Spectrum &s)
	{
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			c[i] += w * s.c[i];
		}
	}

	bool operator==(const Spectrum &sp) const
	{
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			if (c[i] != sp.c[i]) return false;
		}
		return true;
	}

	bool Black() const
	{
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			for (int i = 0; i < COLOR_SAMPLES; ++i)
			{
				if (c[i] != 0.) return false;
			}
			return true;
		}
	}

	float y() const
	{
		float v = 0.;
		for (int i = 0; i < COLOR_SAMPLES; ++i)
		{
			v += YWeight[i] * c[i];
		}
		return v;
	}

	friend Spectrum Exp(const Spectrum &s);

private:

	float c[COLOR_SAMPLES];
	static float XWeight[COLOR_SAMPLES];
	static float YWeight[COLOR_SAMPLES];
	static float ZWeight[COLOR_SAMPLES];

};

Spectrum Exp(const Spectrum &s) 
{
	Spectrum ret;
	for (int i = 0; i < COLOR_SAMPLES; ++i)
		ret.c[i] = expf(s.c[i]);
	return ret;
}

float Spectrum::XWeight[COLOR_SAMPLES] = 
{
	0.412453f, 0.357580f, 0.180423f
};

float Spectrum::YWeight[COLOR_SAMPLES] = 
{
	0.212671f, 0.715160f, 0.072169f
};

float Spectrum::ZWeight[COLOR_SAMPLES] = 
{
	0.019334f, 0.119193f, 0.950227f
};

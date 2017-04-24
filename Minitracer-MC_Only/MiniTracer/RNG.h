
#pragma once

class RNG
{
public:
	RNG(unsigned long long _seed = 7563231ULL)
	{
		seed = _seed;
		mult = 62089911ULL;
		llong_max = 4294967295ULL;
		double_max = 4294967295.0f;
	}

	unsigned long long seed;
	unsigned long long mult;
	unsigned long long llong_max;
	double double_max;

	inline double operator ()()
	{
		seed = mult * seed;
		return double(seed % llong_max) / double_max;
	}
};
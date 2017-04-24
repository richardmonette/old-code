
#ifndef UTILS
#define UTILS 1

#include "Vector3.h"
#include "Vector2.h"

#include <math.h>
#include <iostream>

using namespace std;

static void StringExplode(string str, string separator, vector<string>* results)
{
	int found;

	found = str.find_first_of(separator);

	while(found != string::npos)
	{
		if(found > 0)
		{
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}

	if(str.length() > 0)
	{
		results->push_back(str);
	}
}

static void Report(const char *message)
{
	cout << message << ".\n";
}

static Vector3 UniformSampleSphere(float u1, float u2)
{
	float z = 1.f - 2.f * u1;
	float r = sqrtf(max(0.f, 1.f - z*z));
	float phi = 2.f * 3.14159265 * u2;
	float x = r * cosf(phi);
	float y = r * sinf(phi);
	return Vector3(x, y, z);
}

static float Halton(int id, int prime)
{
	float h = 0.0;
	float f = 1.0/prime;
	float fct = 1.0;
	while(id>0)
	{
		fct *= f;  
		h += (id%prime)*fct; 
		id /= prime;
	}
	return h;
}

static void tentFilter(Vector2 *samples, int num_samples)
{
	for (int i = 0; i < num_samples; i++)
	{
		float x = samples[i].x();
		float y = samples[i].y();

		if (x < 0.5f) samples[i].setX((float)sqrt(2.0*(double)x) - 1.0f);
		else samples[i].setX(1.0f - (float)sqrt(2.0 - 2.0 * (double)x));

		if (y < 0.5f) samples[i].setY((float)sqrt(2.0*(double)y) - 1.0f);
		else samples[i].setY(1.0f - (float)sqrt(2.0 - 2.0 * (double)y));
	}
}

#endif
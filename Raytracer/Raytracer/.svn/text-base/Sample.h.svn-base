
#pragma once

#include "SurfaceIntegrator.h"
#include "VolumeIntegrator.h"
#include "Scene.h"
#include "GlobalDefine.h"

struct Sample
{
	Sample::Sample(SurfaceIntegrator *surf, VolumeIntegrator *vol, const Scene *scene)
	{
		surf->RequestSamples(this, scene);
		vol->RequestSamples(this, scene);

		int nPtrs = n1D.size() + n2D.size();
		if (!nPtrs)
		{
			oneD = twoD = NULL;
			return;
		}
		oneD = (float **)AllocAligned(nPtrs * sizeof(float *));
		twoD = oneD + n1D.size();

		int totSamples = 0;

		for (unsigned int i = 0; i < n1D.size(); ++i)
		{
			totSamples += n1D[i];
		}

		for (unsigned int i = 0; i < n2D.size(); ++i)
		{
			totSamples += 2 * n2D[i];
		}

		float *mem = (float *)AllocAligned(totSamples * sizeof(float));

		for (unsigned int i = 0; i < n1D.size(); ++i)
		{
			oneD[i] = mem;
			mem += n1D[i];
		}

		for (unsigned int i = 0; i < n2D.size(); ++i)
		{
			twoD[i] = mem;
			mem += 2 * n2D[i];
		}
	}

	~Sample(); // ERROR ERROR ERROR THERE IS NO MEMORY FREEING HAPPENING HERE RIGHT NOW

	unsigned int Add1D(unsigned int num)
	{
		n1D.push_back(num);
		return n1D.size()-1;
	}

	unsigned int Add2D(unsigned int num)
	{
		n2D.push_back(num);
		return n2D.size()-1;
	}

	virtual int RoundSize(int size) const = 0;

	float imageX, imageY;
	float lensU, lensV;
	float time;

	vector<unsigned int> n1D, n2D;
	float **oneD, **twoD;

};
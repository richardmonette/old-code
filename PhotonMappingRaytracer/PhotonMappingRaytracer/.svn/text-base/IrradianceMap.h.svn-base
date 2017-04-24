#pragma once

#include "Vector3.h"

typedef struct IrradianceSample 
{
	// Precomputed Irradiances Fields (Christensen)

	Vector3 pos;
	Vector3 power;
	Vector3 normal; // normal of the surface that the photon rests upon
	short plane;	// splitting plane for kd-tree

} IrradianceSample;

typedef struct NearestSample 
{
	Vector3 pos;
	float dist2;
	const IrradianceSample *index;

} NearestSample;

class IrradianceMap
{

public:

	IrradianceMap(const int max_samp);
	~IrradianceMap();

	void store(const float power[3],			// photon power
			const float pos[3],					// photon position
			const float n[3]);  				// photon direction 

	void locate_photon(NearestSample *const np, const int index) const;

	void nearest_precomputed_irradiance(float irradiance[3], const float pos[3], float normal[3]);

	void balance(void);              // balance the kd-tree (before use!)

private:

	void balance_segment(
						 IrradianceSample **pbal,
						 IrradianceSample **porg,
						 const int index,
						 const int start,
						 const int end );

	void median_split(
					  IrradianceSample **p,
					  const int start,
					  const int end,
					  const int median,
					  const int axis );

	IrradianceSample *samples;

	int stored_samples;
	int half_stored_samples;
	int max_samples;
	int prev_scale;

	float bbox_min[3];		// use bbox_min;
	float bbox_max[3];		// use bbox_max;
};


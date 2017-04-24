
#ifndef NORMALS_GENERATOR_H
#define NORMALS_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Types.h"

#define PI 3.141592654

#ifdef __MINGW32__
void *alloca(size_t);
#endif

typedef struct Point 
{
	float pos[3];					// photon position
	short plane;					// splitting plane for kd-tree
	int vertIndex;
	float power;
} Point;

typedef struct NearestPoints 
{
	int max;
	int found;
	int got_heap;
	float pos[3];
	float *dist2;
	Point **index;
} NearestPoints;

class NormalsGenerator 
{

public:
	NormalsGenerator( int max_phot );
	~NormalsGenerator();

	void store(int vertIndex,
			   const float pos[3]);


	void balance(void);              // balance the kd-tree (before use!)

	void NormalsGenerator :: normal_estimate(
  float normal[3],                // returned irradiance
  const float pos[3],            // surface position
  const float max_dist,          // max distance to look for photons
  const int npoints );

	void locate_points(
						NearestPoints *const np,      // np is used to locate the photons
						const int index ) const;       // call with index = 1


	Point* get_points() { return points; }
	int get_stored_points() { return stored_points; }

private:

	void balance_segment(
						 Point **pbal,
						 Point **porg,
						 const int index,
						 const int start,
						 const int end );

	void median_split(
					  Point **p,
					  const int start,
					  const int end,
					  const int median,
					  const int axis );

	Point *points;

	int stored_points;
	int half_stored_points;
	int max_points;
	int prev_scale;

	float bbox_min[3];		// use bbox_min;
	float bbox_max[3];		// use bbox_max;
};

#endif // NORMALS_GENERATOR_H

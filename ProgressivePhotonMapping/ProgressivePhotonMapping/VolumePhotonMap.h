
#ifndef VOLUME_PHOTON_MAP_H
#define VOLUME_PHOTON_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592654

#ifdef __MINGW32__
void *alloca(size_t);
#endif

typedef struct VolumePhoton 
{
	float pos[3];					// photon position
	short plane;					// splitting plane for kd-tree
	float power[3];					// photon power (uncompressed)
} VolumePhoton;

typedef struct NearestVolumePhotons 
{
	int max;
	int found;
	int got_heap;
	float pos[3];
	float *dist2;
	const VolumePhoton **index;
} NearestVolumePhotons;

class VolumePhotonMap 
{

public:
	VolumePhotonMap( int max_phot );
	~VolumePhotonMap();

	void store(const float power[3],				// photon power
			   const float pos[3]);				

	void scale_photon_power(const float scale );           // 1/(number of emitted photons)

	void balance(void);              // balance the kd-tree (before use!)

	int irradiance_estimate(
							 float irrad[3],                // returned irradiance
							 const float pos[3],            // surface position
							 const float max_dist,          // max distance to look for photons
							 const int nphotons	) const;    // number of photons to use

	void locate_photons(
						NearestVolumePhotons *const np,      // np is used to locate the photons
						const int index ) const;       // call with index = 1

	VolumePhoton* get_photons() { return photons; }
	int get_stored_photons() { return stored_photons; }

	NearestVolumePhotons np;

private:

	void balance_segment(
						 VolumePhoton **pbal,
						 VolumePhoton **porg,
						 const int index,
						 const int start,
						 const int end );

	void median_split(
					  VolumePhoton **p,
					  const int start,
					  const int end,
					  const int median,
					  const int axis );

	VolumePhoton *photons;

	int stored_photons;
	int half_stored_photons;
	int max_photons;
	int prev_scale;

	float bbox_min[3];		// use bbox_min;
	float bbox_max[3];		// use bbox_max;
};

#endif // PHOTON_MAP

//----------------------------------------------------------------------------
//
// From Henrik Wann Jensen
//
//----------------------------------------------------------------------------

#ifndef PHOTON_MAP_H
#define PHOTON_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592654

#ifdef __MINGW32__
void *alloca(size_t);
#endif

typedef struct Photon {
	float pos[3];				  // photon position
	short plane;                  // splitting plane for kd-tree
	unsigned char theta, phi;     // incoming direction
	float power[3];               // photon power (uncompressed)
} Photon;

typedef struct NearestPhotons {
	int max;
	int found;
	int got_heap;
	float pos[3];
	float *dist2;
	const Photon **index;
} NearestPhotons;

class PhotonMap {

public:
	PhotonMap( int max_phot );
	~PhotonMap();

	void store(const float power[3],          // photon power
			   const float pos[3],            // photon position
			   const float dir[3] );          // photon direction

	void scale_photon_power(const float scale );           // 1/(number of emitted photons)

	void balance(void);              // balance the kd-tree (before use!)

	void irradiance_estimate(
							 float irrad[3],                // returned irradiance
							 const float pos[3],            // surface position
							 const float normal[3],         // surface normal at pos
							 const float max_dist,          // max distance to look for photons
							 const int nphotons	) const; // number of photons to use

	void locate_photons(
						NearestPhotons *const np,      // np is used to locate the photons
						const int index ) const;       // call with index = 1

	void photon_dir(
					float *dir,                    // direction of photon (returned)
					const Photon *p ) const;       // the photon

	Photon* get_photons() { return photons; }
	int get_stored_photons() { return stored_photons; }

private:

	void balance_segment(
						 Photon **pbal,
						 Photon **porg,
						 const int index,
						 const int start,
						 const int end );

	void median_split(
					  Photon **p,
					  const int start,
					  const int end,
					  const int median,
					  const int axis );

	Photon *photons;

	int stored_photons;
	int half_stored_photons;
	int max_photons;
	int prev_scale;

	float costheta[256];
	float sintheta[256];
	float cosphi[256];
	float sinphi[256];

	float bbox_min[3];		// use bbox_min;
	float bbox_max[3];		// use bbox_max;
};

#endif // PHOTON_MAP


#ifndef RENDERER_H
#define RENDERER_H

#include "RandomMwc.hpp"
#include "rgb.h"
#include "Ray.h"
#include "Scene.h"
#include "IrradianceCache.h"
#include "IrradianceMap.h"
#include "PhotonMap.h"
#include <Windows.h>

class Renderer 
{
public:

	Renderer(int smd) 
	{ 
		smax_depth = smd; 
	}

	rgb directLight(const Ray& r, const Scene* s, const Vector2&, const SurfaceHitRecord& rec, const float nShadowSamples) const;

	rgb color(     
		const Ray& r,       // Ray being sent
		const Scene* s,
		float tmin,         // minimum hit parameter to be searched for
		float tmax,         // maximum hit parameter to be searched for
		Vector2& seed1,     // direct lighting dseed
		Vector2& seed2,     // reflection seed
		bool directLightingViaPhotons,
		int spec_depth,
		bool CEL,           // count emitted light?
		bool finalGather,
	    float &intersect_distance,
		const float screen_x,
		const float screen_y,
		const float nShadowSamples
	) const;

	int smax_depth;

	// Pointers back to instances in the core

	PhotonMap *causticPhotons;
	PhotonMap *directPhotons;
	PhotonMap *indirectPhotons;

	IrradianceCache *irradianceCache;

	IrradianceMap *irradianceMap;

private:

};

#endif

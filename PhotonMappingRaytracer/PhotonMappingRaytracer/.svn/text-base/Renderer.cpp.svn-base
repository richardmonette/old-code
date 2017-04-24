
#include "Renderer.h"
#include "Material.h"
#include "RandomMwc.hpp"
#include "Sample2.h"
#include "Util.h"

#include <stdio.h>
#include <float.h>

rgb Renderer::color(        
      const Ray& r,             // Ray being sent
      const Scene* s,
      float tmin,              // minimum hit parameter to be searched for
      float tmax,              // maximum hit parameter to be searched for
      Vector2& sseed,
      Vector2& rseed,
      bool directLightingViaPhotons,
      int spec_depth,
      bool CEL,           // count emitted light?
	  bool finalGather,
	  float &intersect_distance,
	  const float screen_x,
	  const float screen_y, 
	  const float nShadowSamples) const 
{
	SurfaceHitRecord rec;
	bool cel;
	float brdf_scale;
	rgb c(0,0,0);

	intersect_distance = -666;

	// check if a surface is intersected
	if ( s->surfaces->hit(r, tmin, tmax, rec) ) 
	{
		intersect_distance = (rec.p - r.origin()).length();

		const Vector3 normal = rec.uvw.w();

		Vector3 v_out;
		rgb R;
      
		// find emitted radiance
		if (CEL) c += rec.mat_ptr->emittedRadiance(rec.uvw, -r.direction(), rec.texp, rec.uv);
      
		// trace reflected ray
		if (rec.mat_ptr->scatterDirection(r.direction(), rec, rseed, R, cel, brdf_scale, v_out))
		{
			if (!cel)  // diffuse scatter
			{ 
				if (directLightingViaPhotons)
				{
					float irradiance[3] = {0,0,0};
					float pos[3] = {rec.p.x(), rec.p.y(), rec.p.z()};
					float n[3] = {normal.x(), normal.y(), normal.z()};
					float max_dist = 5.0f;
					int num_photons = 100;

					directPhotons->irradiance_estimate(irradiance, pos, n, max_dist, num_photons);

					c += rgb(irradiance[0], irradiance[1], irradiance[2]) * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv);
				}
				else
				{
					c += brdf_scale * directLight(r, s, sseed, rec, nShadowSamples);
				}
			}
			else       // specular scatter
			{
				if (spec_depth < smax_depth)
				{
					Ray ref(rec.p, v_out);
					c += brdf_scale * R * color(ref, s, 0.001f, FLT_MAX, sseed, rseed, directLightingViaPhotons, spec_depth+1, cel, finalGather, intersect_distance, screen_x, screen_y, nShadowSamples);
				}
			}
		}

		// trace transmitted ray
		if (rec.mat_ptr->transmitDirection(r.direction(), rec, rseed, R, cel, brdf_scale, v_out, 0.0f))
		{
			if (spec_depth < smax_depth)
			{
				Ray ref(rec.p, v_out);
				c += brdf_scale * R * color(ref, s, 0.001f, FLT_MAX, sseed, rseed, directLightingViaPhotons, spec_depth+1, cel, finalGather, intersect_distance, screen_x, screen_y, nShadowSamples);
			}
		}

		if ( !rec.mat_ptr->isSpecular() )
		{ 
			// Get caustic map contribution

			float irradiance[3] = {0,0,0};
			float pos[3] = {rec.p.x(), rec.p.y(), rec.p.z()};
			float n[3] = {normal.x(), normal.y(), normal.z()};
			float max_dist = 10.0f;
			int num_photons = 100;

			//causticPhotons->irradiance_estimate(irradiance, pos, n, max_dist, num_photons);

			c += rgb(irradiance[0], irradiance[1], irradiance[2]) * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv);

			// Get indirect lighting contribution

			finalGather = false;
			if ( finalGather )
			{
				rgb fgColor(0,0,0);

				rgb sum_weightsByValues(0,0,0);
				float sum_weights = 0;
				int entriesUsed = 0;

				irradianceCache->GetCacheValue(entriesUsed, sum_weightsByValues, sum_weights, irradianceCache->octree, normal / normal.length(), rec.p);

				if (entriesUsed > 0)
				{
					fgColor = sum_weightsByValues / sum_weights;

					if (fgColor.r() > 0)
					{
						c += fgColor * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv);
					}		
				}
				else
				{
					const int gatherSamples = 256;
				
					Vector3 fgRayDirection;
					Ray fgRay;

					HamSample2 *sampler = new HamSample2(gatherSamples);
					Vector2 *samples = new Vector2[gatherSamples];
					sampler->generate(samples);

					float harmonicMeanRayDistance = 0.0f;

					for (int m = 0; m < gatherSamples; m++)
					{
						Vector3 newVec = UniformSampleSphere(samples[m][0], samples[m][1]);

						fgRayDirection.setX(normal[0] + newVec[0]);
						fgRayDirection.setY(normal[1] + newVec[1]);
						fgRayDirection.setZ(normal[2] + newVec[2]);

						fgRayDirection = unitVector(fgRayDirection);

						fgRay.setOrigin(rec.p);
						fgRay.setDirection(fgRayDirection);

						float dist = 0.0f;

						fgColor += color(fgRay, s, 0.01, FLT_MAX, sseed, rseed, true, 666, false, false, dist, screen_x, screen_y, 4.0f);

						if (dist != -666)
						{
							harmonicMeanRayDistance += 1.0f / dist; 
						}
					}

					delete sampler;
					delete samples;

					c += (fgColor / float(gatherSamples)) * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv);

					IrrCacheEntry *cacheEntry = new IrrCacheEntry();

					cacheEntry->normal = normal / normal.length();
					cacheEntry->position = rec.p;
					cacheEntry->radius = float(gatherSamples) / harmonicMeanRayDistance;

					const float minDist = sqrtf(intersect_distance);

					cacheEntry->radius = min(cacheEntry->radius, minDist);

					cacheEntry->radius = max(cacheEntry->radius, 10.0f);

					cacheEntry->value = (fgColor / float(gatherSamples)) * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv);
					cacheEntry->screenPosition = Vector2(screen_x, screen_y);

					irradianceCache->AddCacheEntry(cacheEntry);
				}
			}
			else
			{
				float irradiance[3];
				float pos[3] = {rec.p.x(), rec.p.y(), rec.p.z()};
				float n[3] = {normal.x(), normal.y(), normal.z()};
					
				float max_dist = 2.0f;
				int num_photons = 200;
				indirectPhotons->irradiance_estimate(irradiance, pos, n, max_dist, num_photons);
		
				//irradianceMap->nearest_precomputed_irradiance(irradiance, pos, n);

				c += rgb(irradiance[0], irradiance[1], irradiance[2]) * rec.mat_ptr->ambientResponse(rec.uvw, r.direction(), rec.p, rec.uv) * 1.0f;
			}
		}

		return c;
	}
	else
	{
		return s->background->value( Vector2(0,0), r.direction() );
	}
}

rgb Renderer::directLight(const Ray& r,
                          const Scene* s,       
                          const Vector2& seed,
                          const SurfaceHitRecord& rec,
						  const float nShadowSamples) const 
{
	rgb c(0,0,0);

	int cid = 0;

	for (int i = 0; i < s->lights.length(); i++) 
	{
		Vector3 on_light;
		float pdf;
		rgb E;
		Vector3 N;

		float scale = 1.0f / nShadowSamples;

		for (int x = 0; x < nShadowSamples; x++)
		{
			Vector2 v1(Halton(cid, 2), Halton(cid, 3));
			cid++;

			if (s->lights[i]->randomPoint(rec.p, v1, on_light, N, pdf, E)) 
			{
				Ray sr(rec.p, on_light - rec.p);
				double dist = sr.direction().length();
				double cosine0 = dot(sr.direction(), rec.uvw.w()) / dist;
				if (cosine0 > 0.0f && !s->surfaces->shadowHit(sr, 0.00001f, 0.9999f) ) 
				{
					rgb brdf;
					if (rec.mat_ptr->explicitBrdf(rec.uvw, -r.direction(), sr.direction(), rec.texp, rec.uv, brdf)) 
					{
						float cosine1 = dot(-sr.direction(), N) / dist;

						if (cosine1 > 0.0f) 
						{
							c += brdf * E * (cosine0*cosine1/(dist*dist*pdf)) * scale;
						}
					}
				}
			}
		}
	}

	return c;
}



/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/

#include "math.h"

#include "Vector3f.hpp"
#include "Scene.hpp"
#include "SurfacePoint.hpp"
#include "RandomMwc.hpp"

#include "RayTracer.hpp"

using namespace hxa7241_minilight;

RayTracer::RayTracer (const Scene& scene) : pScene_m( &scene )
{
}

Vector3f RayTracer::getRadiance
(
   const Vector3f&  rayOrigin,
   const Vector3f&  rayDirection,
   const RandomMwc& random,
   const int iterations,
   const void*      lastHit,
   const bool       reflRefrRay
) const
{
   // intersect ray with scene
   const Triangle* pHitObject = 0;
   Vector3f        hitPosition;
   Vector3f        hitUVT;
   pScene_m->getIntersection( rayOrigin, rayDirection, lastHit, pHitObject, hitPosition, hitUVT );

   Vector3f radiance;

   if ( iterations > 5 ) return radiance;

   if( 0 != pHitObject )
   {
		// make surface point of intersection
		SurfacePoint surfacePoint( *pHitObject, hitPosition, hitUVT.getX(), hitUVT.getY() );
		
		if ( pHitObject->getMaterial() == 1 ) // REFLECTION
		{
			  Vector3f nextDirection;
			  Vector3f color;
			  // check surface bounces ray
			  if( surfacePoint.getNextDirectionSpecular( random, -rayDirection, nextDirection, color ) )
			  {
				 // recurse
				 radiance = radiance + (color * getRadiance( surfacePoint.getPosition(), nextDirection, random, iterations + 1, surfacePoint.getHitId(), true ));
			  }
		}
		else if ( pHitObject->getMaterial() == 2 ) // REFRACTION
		{
			const float reflectivityMean = pHitObject->getReflectivity(hitUVT.getX(), hitUVT.getY()).dot( Vector3f::ONE() ) / 3.0f;
			
			Vector3f color = pHitObject->getReflectivity(hitUVT.getX(), hitUVT.getY());
			Vector3f n( pHitObject->getNormalBarycentric(hitUVT.getX(), hitUVT.getY()) );

			const float n1 = 1.0f;
			const float n2 = 1.5f;

			const bool into = rayDirection.dot( n ) > 0;
			const float ni = into ? n2 / n1 : n1 / n2; 

			const float cosTheta1 = n.dot(-rayDirection);
			const float cosTheta2 = 1.0f - ni * ni * ( 1.0f - cosTheta1 * cosTheta1 );

			if (cosTheta2 < 0) // Total internal reflection
			{
				const Vector3f reflectionVector = rayDirection + n * ( 2 * cosTheta1 );

				radiance = radiance + color * getRadiance(hitPosition, reflectionVector, random, iterations + 1, surfacePoint.getHitId(), true);
			}

			const Vector3f refractionVector = (rayDirection * ni) + n * ( (cosTheta1 * ni) + (cosTheta1 > 0.0 ? -sqrtf(cosTheta2) : sqrtf(cosTheta2) ) );

			const Vector3f refraction = getRadiance(hitPosition, refractionVector, random, iterations + 1, surfacePoint.getHitId(), true);

			const Vector3f reflectionVector = rayDirection + n * ( 2 * cosTheta1 );

			const Vector3f reflection = getRadiance(hitPosition, reflectionVector, random, iterations + 1, surfacePoint.getHitId(), true);

			radiance = radiance + (refraction + reflection) * 0.5f;
		}
		else // DEFAULT DIFFUSE
		{
			const int shadowSamples = 16;

			for (int i = 0; i < shadowSamples; i++)
			{
				radiance = radiance + sampleEmitters( rayDirection, surfacePoint, random, pHitObject);
			}

			radiance = radiance / shadowSamples;

			radiance = radiance + surfacePoint.getEmission( rayOrigin, -rayDirection, false);
		}

   }
   else
   {
		// no hit: default/background scene emission
		radiance = pScene_m->getDefaultEmission( -rayDirection );
   }

   return radiance;
}

Vector3f RayTracer::sampleEmitters( const Vector3f& rayDirection, const SurfacePoint& surfacePoint, const RandomMwc& random, const Triangle *object ) const
{
	Vector3f radiance;

	Vector3f        emitterPosition;
	const Triangle* emitterId = 0;
	pScene_m->getEmitter( random, emitterPosition, emitterId );

	if( 0 != emitterId )
	{
		const Vector3f emitDirection( (emitterPosition - surfacePoint.getPosition()).unitize() );

		const Triangle* pHitObject = 0;
		Vector3f        hitPosition;
		Vector3f        hitUVT;
		pScene_m->getIntersection( surfacePoint.getPosition(), emitDirection, surfacePoint.getHitId(), pHitObject, hitPosition, hitUVT );

		if( (0 == pHitObject) | (emitterId == pHitObject) )
		{
			Vector3f normal = object->getNormalBarycentric(hitUVT.getX(), hitUVT.getY());

			const float ndotl = normal.dot(emitDirection);

			radiance = object->getReflectivity(hitUVT.getX(), hitUVT.getY()) * ndotl;
		}
	}

	return radiance;
}

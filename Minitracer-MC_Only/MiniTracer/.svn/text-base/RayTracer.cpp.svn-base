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




/// standard object services ---------------------------------------------------
RayTracer::RayTracer
(
   const Scene& scene
)
 : pScene_m( &scene )
{
}




/// queries --------------------------------------------------------------------
Vector3f RayTracer::getRadiance
(
   const Vector3f&  rayOrigin,
   const Vector3f&  rayDirection,
   const RandomMwc& random,
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
   if( 0 != pHitObject )
   {
      // make surface point of intersection
	   SurfacePoint surfacePoint( *pHitObject, hitPosition, hitUVT.getX(), hitUVT.getY() );

      // local emission only for first-hit
      //radiance = ( 0 != lastHit ? Vector3f::ZERO() : surfacePoint.getEmission( rayOrigin, -rayDirection, false ) );
	   if ( lastHit == 0 || reflRefrRay == true )
		radiance = radiance + surfacePoint.getEmission( rayOrigin, -rayDirection, false); // WE DONT WANT THIS BIAS
	   

      // add emitter sample
	  //if ( !reflRefrRay )
		//radiance = radiance + sampleEmitters( rayDirection, surfacePoint, random);
		
		if ( pHitObject->getMaterial() == 1 ) // REFLECTION
		{
			  Vector3f nextDirection;
			  Vector3f color;
			  // check surface bounces ray
			  if( surfacePoint.getNextDirectionSpecular( random, -rayDirection, nextDirection, color ) )
			  {
				 // recurse
				 radiance = radiance + (color * getRadiance( surfacePoint.getPosition(), nextDirection, random, surfacePoint.getHitId(), true ));
			  }
		}
		else if ( pHitObject->getMaterial() == 2 ) // REFRACTION
		{
			const float reflectivityMean = pHitObject->getReflectivity(hitUVT.getX(), hitUVT.getY()).dot( Vector3f::ONE() ) / 3.0f;
			
			Vector3f color;
			if( random.getFloat() < reflectivityMean ) // russian-roulette for reflectance magnitude
			{
				color = pHitObject->getReflectivity(hitUVT.getX(), hitUVT.getY()) / reflectivityMean;
				Vector3f n( pHitObject->getNormalBarycentric(hitUVT.getX(), hitUVT.getY()) );

				// Use Snell's law(s) to figure out the reflection and refraction vectors

				const float n1 = 1.0f;
				const float n2 = 1.5f;

				const bool into = rayDirection.dot( n ) > 0;
				const float ni = into ? n2 / n1 : n1 / n2; 

				const float cosTheta1 = n.dot(-rayDirection);
				const float cosTheta2 = 1.0f - ni * ni * ( 1.0f - cosTheta1 * cosTheta1 );

				const Vector3f reflectionVector = rayDirection + n * ( 2 * cosTheta1 );

				if (cosTheta2 < 0) // Total internal reflection
				{
					radiance = radiance + color * getRadiance(hitPosition, reflectionVector, random, surfacePoint.getHitId(), true);
				}

				//Vec tdir = (ray.direction*nnt - intersectNormal*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
				const Vector3f refractionVector = (rayDirection * ni) + n * ( (cosTheta1 * ni) + (cosTheta1 > 0.0 ? -sqrtf(cosTheta2) : sqrtf(cosTheta2) ) );

				//const double a = n2 - n1;
				//const double b = n2 + n1;
				/*const double R0 = a * a / (b * b);
				const double c = 1 - ( into ? -cosTheta1 : refractionVector.dot( n ) );
				const double Re = R0 + (1 - R0) * c * c * c * c * c;
				const double Tr = 1 - Re;*/

				//const float Tr = ( 4.0 * n1 * n2 ) / ( b * b );
			
				/*if ( random.getFloat() > 0.5f )
				{
					radiance = radiance + color * getRadiance(hitPosition, reflectionVector, random, surfacePoint.getHitId());// * Re;
				}
				else
				{*/
					radiance = radiance + color * getRadiance(hitPosition, refractionVector, random, surfacePoint.getHitId(), true);// * Tr;
				//}				
			}
		}
		else // DEFAULT DIFFUSE
		{
			radiance = radiance + sampleEmitters( rayDirection, surfacePoint, random);

			// add recursive reflection
			  //
			  // single hemisphere sample, ideal diffuse BRDF:
			  // reflected = (inradiance * pi) * (cos(in) / pi * color) * reflectance
			  // -- reflectance magnitude is 'scaled' by the russian roulette, cos is
			  // importance sampled (both done by SurfacePoint), and the pi and 1/pi
			  // cancel out
			  Vector3f nextDirection;
			  Vector3f color;
			  // check surface bounces ray
			  if( surfacePoint.getNextDirectionDiffuse( random, -rayDirection, nextDirection, color ) )
			  {
				 // recurse
				 radiance = radiance + (color * getRadiance( surfacePoint.getPosition(), nextDirection, random, surfacePoint.getHitId() ));
			  }
		}

   }
   else
   {
      // no hit: default/background scene emission
      radiance = pScene_m->getDefaultEmission( -rayDirection );
   }

   return radiance;
}




/// implementation -------------------------------------------------------------
Vector3f RayTracer::sampleEmitters
(
   const Vector3f&     rayDirection,
   const SurfacePoint& surfacePoint,
   const RandomMwc&    random
) const
{
   Vector3f radiance;

   // single emitter sample, ideal diffuse BRDF:
   // reflected = (emitivity * solidangle) * (emitterscount) *
   // (cos(emitdirection) / pi * reflectivity)
   // -- SurfacePoint does the first and last parts (in separate methods)

   // get position on an emitter
   Vector3f        emitterPosition;
   const Triangle* emitterId = 0;
   pScene_m->getEmitter( random, emitterPosition, emitterId );

   // check an emitter was found
   if( 0 != emitterId )
   {
      // make direction to emit point
      const Vector3f emitDirection( (emitterPosition -
         surfacePoint.getPosition()).unitize() );

      // send shadow ray
      const Triangle* pHitObject = 0;
      Vector3f        hitPosition;
	  Vector3f        hitUVT;
      pScene_m->getIntersection( surfacePoint.getPosition(), emitDirection,
         surfacePoint.getHitId(), pHitObject, hitPosition, hitUVT );

      // if unshadowed, get inward emission value
      Vector3f emissionIn;
      if( (0 == pHitObject) | (emitterId == pHitObject) )
      {
		  emissionIn = SurfacePoint( *emitterId, emitterPosition, hitUVT.getX(), hitUVT.getY() ).getEmission(
            surfacePoint.getPosition(), -emitDirection, true );
      }

      // get amount reflected by surface
      radiance = surfacePoint.getReflection( emitDirection, emissionIn * static_cast<float>(pScene_m->getEmittersCount()), -rayDirection );
   }

   return radiance;
}

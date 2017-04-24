/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/

#include <math.h>

#include "RandomMwc.hpp"
#include "Triangle.hpp"

#include "SurfacePoint.hpp"

using namespace hxa7241_minilight;

namespace
{
   const float PI = 3.14159265358979f;
}

SurfacePoint::SurfacePoint
(
   const Triangle& triangle,
   const Vector3f& position,
   const float u,
   const float v
)
 : pTriangle_m( &triangle )
 , position_m ( position )
 , u_m( u )
 , v_m( v )
{
}

Vector3f SurfacePoint::getEmission
(
   const Vector3f& toPosition,
   const Vector3f& outDirection,
   const bool      isSolidAngle
) const
{
   const Vector3f ray( toPosition - position_m );
   const float    distance2 = ray.dot( ray );
   const float    cosArea   = outDirection.dot( pTriangle_m->getNormal() ) * pTriangle_m->getArea();

   // clamp-out infinity
   const float solidAngle = isSolidAngle ? cosArea / (distance2 >= 1e-6f ? distance2 : 1e-6f) : 1.0f;

   // emit from front face of surface only
   return Vector3f( cosArea > 0.0f ? pTriangle_m->getEmitivity(u_m, v_m) * solidAngle : Vector3f::ZERO() );
}

/*Vector3f SurfacePoint::getReflection
(
   const Vector3f& inDirection,
   const Vector3f& inRadiance,
   const Vector3f& outDirection
) const
{
   const float inDot  = inDirection.dot(  pTriangle_m->getNormalBarycentric(u_m, v_m) );
   const float outDot = outDirection.dot( pTriangle_m->getNormalBarycentric(u_m, v_m) );

   // directions must be on same side of surface
   return Vector3f( (inDot < 0.0f) ^ (outDot < 0.0f) ? Vector3f::ZERO() :
      // ideal diffuse BRDF:
      // radiance scaled by cosine, 1/pi, and reflectivity
      (inRadiance * pTriangle_m->getReflectivity(u_m, v_m)) * (::fabsf( inDot ) / PI) );
}*/

bool SurfacePoint::getNextDirectionDiffuse
(
   const RandomMwc& random,
   const Vector3f&  inDirection,
   Vector3f&        outDirection,
   Vector3f&        color
) const
{
   outDirection = Vector3f::ZERO();

   const float reflectivityMean = pTriangle_m->getReflectivity(u_m, v_m).dot( Vector3f::ONE() ) / 3.0f;

   // russian-roulette for reflectance magnitude
   if( random.getFloat() < reflectivityMean )
   {
      color = pTriangle_m->getReflectivity(u_m, v_m) / reflectivityMean;

      // cosine-weighted importance sample hemisphere

      const float _2pr1 = PI * 2.0f * random.getFloat();
      const float sr2   = ::sqrtf( random.getFloat() );

      // make coord frame coefficients (z in normal direction)
      const float x = ::cosf( _2pr1 ) * sr2;
      const float y = ::sinf( _2pr1 ) * sr2;
      const float z = ::sqrtf( 1.0f - (sr2 * sr2) );

      // make coord frame
      Vector3f normal ( pTriangle_m->getNormalBarycentric(u_m, v_m)  );
      Vector3f tangent( pTriangle_m->getTangent() );
      normal = normal.dot( inDirection ) >= 0.0f ? normal : -normal;

      // make vector from frame times coefficients
      outDirection = (tangent * x) + (normal.cross( tangent ) * y) +
         (normal * z);
   }

   return !outDirection.isZero();
}


bool SurfacePoint::getNextDirectionSpecular
(
   const RandomMwc& random,
   const Vector3f&  inDirection,
   Vector3f&        outDirection,
   Vector3f&        color
) const
{
	outDirection = Vector3f::ZERO();

	color = pTriangle_m->getReflectivity(u_m, v_m);

	// make coord frame
	Vector3f normal ( pTriangle_m->getNormalBarycentric(u_m, v_m) );
	normal = normal.dot( inDirection ) >= 0.0f ? normal : -normal;

	// make vector from frame times coefficients
	outDirection = -(inDirection - normal * 2.0 * normal.dot(inDirection));

	return !outDirection.isZero();
}

const void* SurfacePoint::getHitId() const
{
   return pTriangle_m;
}

Vector3f SurfacePoint::getPosition() const
{
   return position_m;
}

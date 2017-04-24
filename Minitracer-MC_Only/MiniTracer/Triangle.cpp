/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include <math.h>
#include <istream>

#include "RandomMwc.hpp"

#include "Triangle.hpp"
#include "Textures.h"


using namespace hxa7241_minilight;




/// standard object services ---------------------------------------------------
Triangle::Triangle
(
   std::istream& in,
   Textures& textures
)
{

	pTextures_m = &textures;

   // read geometry
   for( dword i = 0;  i < 3;  in >> vertexs_m[i++] ) {}

   //read normals

   for( dword i = 0; i < 3; in >> normals_m[i++] ) {}

   //read uvs

   for( dword i = 0; i < 3; in >> uvs_m[i++] ) {}

   // read and condition quality
   {
      in >> reflectivity_m;
      reflectivity_m = reflectivity_m.getClamped(Vector3f::ZERO(), Vector3f::ONE() );

      in >> emitivity_m;
      emitivity_m = emitivity_m.getClamped( Vector3f::ZERO(), Vector3f::MAX() );

	  in >> material_m;
   }
}




/// queries --------------------------------------------------------------------
void Triangle::getBound
(
   float bound[6]
) const
{
   // initialize
   for( dword i = 6;  i-- > 0;  bound[i] = vertexs_m[2][i % 3] ) {}

   // expand
   for( dword i = 0;  i < 3;  ++i )
   {
      for( dword j = 0, d = 0, m = 0;  j < 6;  ++j, d = j / 3, m = j % 3 )
      {
         // include some tolerance
         const float v = vertexs_m[i][m] + ((d ? 1.0f : -1.0f) *
            (::fabsf(vertexs_m[i][m]) + 1.0f) * TOLERANCE());
         bound[j] = (bound[j] > v) ^ d ? v : bound[j];
      }
   }
}


/**
 * @implementation
 * Adapted from:
 * <cite>'Fast, Minimum Storage Ray-Triangle Intersection'
 * Moller, Trumbore;
 * Journal Of Graphics Tools, v2n1p21, 1997.
 * http://www.acm.org/jgt/papers/MollerTrumbore97/</cite>
 */
bool Triangle::getIntersection
(
   const Vector3f& rayOrigin,
   const Vector3f& rayDirection,
   float&          hitDistance,
   Vector3f&       hitUVT
) const
{
   // find vectors for two edges sharing vert0
   const Vector3f edge1( vertexs_m[1] - vertexs_m[0] );
   const Vector3f edge2( vertexs_m[2] - vertexs_m[0] );

   // begin calculating determinant - also used to calculate U parameter
   const Vector3f pvec( rayDirection.cross( edge2 ) );

   // if determinant is near zero, ray lies in plane of triangle
   const float det = edge1.dot( pvec );

   {
      static const float EPSILON = 0.000001f;
      if( (det > -EPSILON) & (det < EPSILON) )
      {
         return false;
      }

      const float inv_det = 1.0f / det;

      // calculate distance from vertex 0 to ray origin
      const Vector3f tvec( rayOrigin - vertexs_m[0] );

      // calculate U parameter and test bounds
      const float u = tvec.dot( pvec ) * inv_det;
      if( (u < 0.0f) | (u > 1.0f) )
      {
         return false;
      }

      // prepare to test V parameter
      const Vector3f qvec( tvec.cross( edge1 ) );

      // calculate V parameter and test bounds
      const float v = rayDirection.dot( qvec ) * inv_det;
      if( (v < 0.0f) | (u + v > 1.0f) )
      {
         return false;
      }

      // calculate t, ray intersects triangle
      hitDistance = edge2.dot( qvec ) * inv_det;

	  hitUVT.setX(u);
	  hitUVT.setY(v);
   }

   // only allow intersections in the forward ray direction
   return hitDistance >= 0.0f;
}


Vector3f Triangle::getSamplePoint
(
   const RandomMwc& random
) const
{
   // get two randoms
   const float sqr1 = ::sqrtf( random.getFloat() );
   const float r2   = random.getFloat();

   // make barycentric coords
   const float a = 1.0f - sqr1;
   const float b = (1.0f - r2) * sqr1;
   //const float c = r2 * sqr1;

   // make position from barycentrics
   // calculate interpolation by using two edges as axes scaled by the
   // barycentrics
   return Vector3f( ((vertexs_m[1] - vertexs_m[0]) * a) +
      ((vertexs_m[2] - vertexs_m[0]) * b) + vertexs_m[0] );
}


Vector3f Triangle::getNormalBarycentric(const float u, const float v) const
{
	return (normals_m[0] + (normals_m[1] - normals_m[0]) * u + (normals_m[2] - normals_m[0]) * v).unitize();
}


Vector3f Triangle::getNormal() const
{
   return getTangent().cross( vertexs_m[2] - vertexs_m[1] ).unitize();
}


Vector3f Triangle::getTangent() const
{
   return (vertexs_m[1] - vertexs_m[0]).unitize();
}


float Triangle::getArea() const
{
   // half area of parallelogram
   const Vector3f pa2( (vertexs_m[1] - vertexs_m[0]).cross(
      vertexs_m[2] - vertexs_m[1] ) );
   return ::sqrtf( pa2.dot(pa2) ) * 0.5f;
}


Vector3f Triangle::getReflectivity(const float u, const float v) const
{
	if (getTextureIndex() != -1)
	{
		const Vector3f imageUV = (uvs_m[0] + (uvs_m[1] - uvs_m[0]) * u + (uvs_m[2] - uvs_m[0]) * v);
		return pTextures_m->GetSample(getTextureIndex(), imageUV.getX(), imageUV.getY());
	}

	return reflectivity_m;
}


Vector3f Triangle::getEmitivity(float u, float v) const
{
	/*if (getTextureIndex() != -1)
	{
		return pTextures_m->GetSample(getTextureIndex(), u, v);
	}*/

	return emitivity_m;
}

float Triangle::getMaterial() const
{
	return material_m.getX();
}


int Triangle::getTextureIndex() const
{
	return material_m.getY();
}


float Triangle::TOLERANCE()
{
   // one mm seems reasonable...
   return 1.0f / 1024.0f;
}

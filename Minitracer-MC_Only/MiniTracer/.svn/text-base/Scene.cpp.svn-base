/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include <istream>
#include <iostream>

#include "RandomMwc.hpp"
#include "SpatialIndex.hpp"

#include "Scene.hpp"


using namespace hxa7241_minilight;




/// constants ------------------------------------------------------------------
namespace
{
   // 2^20 ~= a million
   const dword  MAX_TRIANGLES  = 0x100000;
   const udword MAX_EMITTERS_P = 16;
}




/// standard object services ---------------------------------------------------
Scene::Scene
(
   std::istream&   in,
   const Vector3f& eyePosition,
   Textures& textures
)
{
   // read and condition default sky and ground values
   in >> skyEmission_m >> groundReflection_m;

   skyEmission_m = skyEmission_m.getClamped( Vector3f::ZERO(), skyEmission_m );
   groundReflection_m = skyEmission_m * groundReflection_m.getClamped(
      Vector3f::ZERO(), Vector3f::ONE() );

   // read objects
   try
   {
      for( dword i = 0;  i < MAX_TRIANGLES;  ++i )
      {
         triangles_m.push_back( Triangle( in, textures ) );
      }
   }
   // EOF is not really exceptional here, but the code is so much simpler.
   catch( std::ios_base::failure )
   //catch( std::ios_base::eof )   // seems to have disappeared unfortunately...
   {
      if( !in.eof() ) throw;
   }

   // find emitting triangles
   for( udword i = 0; (i < triangles_m.size()) &
      (emitters_m.size() < (1u << MAX_EMITTERS_P));  ++i )
   {
      // has non-zero emission and area
      if( !triangles_m[i].getEmitivity(0.5,0.5).isZero() &&
         (triangles_m[i].getArea() > 0.0f) )
      {
         emitters_m.push_back( &(triangles_m[i]) );
      }
   }

   // make index
   pIndex_m = new SpatialIndex( eyePosition, triangles_m );

   std::cout << "Scene completed.\n";
}


Scene::~Scene()
{
   delete pIndex_m;
}




/// queries --------------------------------------------------------------------
void Scene::getIntersection
(
   const Vector3f&  rayOrigin,
   const Vector3f&  rayDirection,
   const void*      lastHit,
   const Triangle*& pHitObject,
   Vector3f&        hitPosition,
   Vector3f&        hitUVT
) const
{
   pIndex_m->getIntersection( rayOrigin, rayDirection, lastHit,
      pHitObject, hitPosition, hitUVT );
}


void Scene::getEmitter
(
   const RandomMwc& random,
   Vector3f&        position,
   const Triangle*& id
) const
{
   if( !emitters_m.empty() )
   {
      // select emitter
      // not using lower bits, by treating the random as fixed-point i.f bits
      const dword index = ((static_cast<udword>(random.getDword()) &
         ((1u << MAX_EMITTERS_P) - 1)) * emitters_m.size()) >> MAX_EMITTERS_P;

      // get position on triangle
      position = emitters_m[ index ]->getSamplePoint( random );
      id       = emitters_m[ index ];
   }
   else
   {
      position = Vector3f::ZERO();
      id       = 0;
   }
}


dword Scene::getEmittersCount() const
{
   return static_cast<dword>(emitters_m.size());
}


Vector3f Scene::getDefaultEmission
(
   const Vector3f& backDirection
) const
{
   // sky for downward ray, ground for upward ray
   return (backDirection[1] < 0.0f) ? skyEmission_m : groundReflection_m;
}

/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef Scene_h
#define Scene_h


#include <iosfwd>
#include <vector>

#include "Vector3f.hpp"
#include "Triangle.hpp"




#include "hxa7241_minilight.hpp"
namespace hxa7241_minilight
{


/**
 * A grouping of the objects in the environment.<br/><br/>
 *
 * Makes a sub-grouping of emitting objects.<br/><br/>
 *
 * Constant.
 *
 * @invariants
 * * triangles_m.size() <= 2^20
 * * emitters_m.size()  <= 2^16
 * * pIndex_m is a non-zero pointer to a SpatialIndex
 * * skyEmission_m      >= 0
 * * groundReflection_m >= 0
 */
class Scene
{
/// standard object services ---------------------------------------------------
public:
            Scene( std::istream&,
                   const Vector3f& eyePosition, Textures& textures );

           ~Scene();
private:
            Scene( const Scene& );
   Scene& operator=( const Scene& );
public:


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           void     getIntersection( const Vector3f&  rayOrigin,
                                     const Vector3f&  rayDirection,
                                     const void*      lastHit,
                                     const Triangle*& pHitObject,
                                     Vector3f&        hitPosition,
									 Vector3f&        hitUVT)       const;

           void     getEmitter( const RandomMwc& random,
                                Vector3f&        position,
                                const Triangle*& id )                     const;
           dword    getEmittersCount()                                    const;

           Vector3f getDefaultEmission( const Vector3f& backDirection )   const;


/// fields ---------------------------------------------------------------------
private:
   std::vector<Triangle>        triangles_m;
   std::vector<const Triangle*> emitters_m;
   const SpatialIndex*          pIndex_m;

   Vector3f skyEmission_m;
   Vector3f groundReflection_m;
};


}//namespace




#endif//Scene_h

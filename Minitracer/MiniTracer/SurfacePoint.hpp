/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef SurfacePoint_h
#define SurfacePoint_h


#include "Vector3f.hpp"




#include "hxa7241_minilight.hpp"
namespace hxa7241_minilight
{


/**
 * Surface point at a ray-object intersection.<br/><br/>
 *
 * All direction parameters are away from surface.<br/><br/>
 *
 * Constant.<br/><br/>
  *
 * @invariants
 * * pTriangle_m points to a Triangle (is not 0)
*/
class SurfacePoint
{
/// standard object services ---------------------------------------------------
public:
            SurfacePoint( const Triangle& triangle,
                          const Vector3f& position,
						  const float u,
						  const float v);

// use defaults
//         ~SurfacePoint();
//          SurfacePoint( const SurfacePoint& );
// SurfacePoint& operator=( const SurfacePoint& );


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           Vector3f getEmission( const Vector3f& toPosition,
                                 const Vector3f& outDirection,
                                 const bool      isSolidAngle)           const;

           /*Vector3f getReflection( const Vector3f& inDirection,
                                   const Vector3f& inRadiance,
                                   const Vector3f& outDirection)         const;*/

           bool     getNextDirectionDiffuse( const RandomMwc& random,
                                      const Vector3f&  inDirection,
                                      Vector3f&        outDirection,
                                      Vector3f&        color
									  )            const;

		   bool     getNextDirectionSpecular( const RandomMwc& random,
                                      const Vector3f&  inDirection,
                                      Vector3f&        outDirection,
                                      Vector3f&        color)            const;

           const void* getHitId()                                         const;
           Vector3f    getPosition()                                      const;


/// fields ---------------------------------------------------------------------
private:
   const Triangle* pTriangle_m;
   Vector3f        position_m;

   float u_m;
   float v_m; // the uv position on the triangle
};


}//namespace




#endif//SurfacePoint_h

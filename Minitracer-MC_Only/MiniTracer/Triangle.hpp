/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef Triangle_h
#define Triangle_h


#include <iosfwd>

#include "Vector3f.hpp"




#include "hxa7241_minilight.hpp"
namespace hxa7241_minilight
{


/**
 * A simple, explicit/non-vertex-shared triangle.<br/><br/>
 *
 * Includes geometry and quality.<br/><br/>
 *
 * Constant.<br/><br/>
 *
 * @implementation
 * Adapts ray intersection code from:
 * <cite>'Fast, Minimum Storage Ray-Triangle Intersection'
 * Moller, Trumbore;
 * Journal of Graphics Tools, v2 n1 p21, 1997.
 * http://www.acm.org/jgt/papers/MollerTrumbore97/</cite>
 *
 * @invariants
 * * reflectivity_m >= 0 and <= 1
 * * emitivity_m    >= 0
 */
class Triangle
{
/// standard object services ---------------------------------------------------
public:
   explicit Triangle( std::istream&, Textures& textures );

// use defaults
//         ~Triangle();
//          Triangle( const Triangle& );
// Triangle& operator=( const Triangle& );


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           void     getBound( float bound[6] )                            const;
           bool     getIntersection( const Vector3f& rayOrigin,
                                     const Vector3f& rayDirection,
                                     float&          hitDistance,
									 Vector3f&       hitUVT)        const;
           Vector3f getSamplePoint( const RandomMwc& random )             const;

           Vector3f getNormal()                                           const;
		   Vector3f getNormalBarycentric(const float u, const float v)    const;
           Vector3f getTangent()                                          const;
           float    getArea()                                             const;

           Vector3f getReflectivity(const float u, const float v)         const;
           Vector3f getEmitivity(float u, float v)                        const;
		   float	getMaterial()										  const;
		   int		getTextureIndex()									  const;

   static  float TOLERANCE();


/// fields ---------------------------------------------------------------------
private:

   // geometry
   Vector3f vertexs_m[3];

   // normals
   Vector3f normals_m[3];

   // uvs
   Vector3f uvs_m[3];

   // quality
   Vector3f reflectivity_m;
   Vector3f emitivity_m;

   // material
   Vector3f material_m;

   Textures* pTextures_m;
};


}//namespace




#endif//Triangle_h

/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#ifndef Camera_h
#define Camera_h


#include <iosfwd>

#include "Vector3f.hpp"




#include "hxa7241_minilight.hpp"
namespace hxa7241_minilight
{


/**
 * A View with rasterization capability.<br/><br/>
 *
 * getFrame() accumulates a frame to the image.<br/><br/>
 *
 * Constant.
 *
 * @invariants
 * * viewAngle_m is >= 10 and <= 160 degrees in radians
 * * viewDirection_m is unitized
 * * right_m is unitized
 * * up_m is unitized
 * * above three form a coordinate frame
 */
class Camera
{
/// standard object services ---------------------------------------------------
public:
   explicit Camera( std::istream& );

// use defaults
//         ~Camera();
//          Camera( const Camera& );
// Camera& operator=( const Camera& );


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           Vector3f getEyePoint()                                         const;

           void  getFrame( const Scene&,
                           const RandomMwc&,
                           Image& )                                       const;


/// fields ---------------------------------------------------------------------
private:
   Vector3f viewPosition_m;
   float    viewAngle_m;

   // view frame
   Vector3f viewDirection_m;
   Vector3f right_m;
   Vector3f up_m;
};


}//namespace




#endif//Camera_h

/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include <math.h>
#include <istream>

#include "RandomMwc.hpp"
#include "RayTracer.hpp"
#include "Image.hpp"
#include "Textures.h"

#include "Camera.hpp"


using namespace hxa7241_minilight;




/// standard object services ---------------------------------------------------
Camera::Camera
(
   std::istream& in
)
{
   // read and condition view definition
   {
      in >> viewPosition_m >> viewDirection_m >> viewAngle_m;

      viewDirection_m = viewDirection_m.unitize();
      if( viewDirection_m.isZero() )
      {
         viewDirection_m = Vector3f( 0.0f, 0.0f, 1.0f );
      }

      viewAngle_m = (viewAngle_m < 10.0f ? 10.0f : (viewAngle_m > 160.0f ?
         160.0f : viewAngle_m)) * (3.14159265358979f / 180.0f);
   }

   // make other directions of frame
   {
      up_m    = Vector3f( 0.0f, 1.0f, 0.0f );
      right_m = up_m.cross( viewDirection_m ).unitize();

      if( !right_m.isZero() )
      {
         up_m = viewDirection_m.cross( right_m ).unitize();
      }
      else
      {
         up_m = Vector3f( 0.0f, 0.0f, viewDirection_m[1] < 0.0f ? 1.0f : -1.0f);
         right_m = up_m.cross( viewDirection_m ).unitize();
      }
   }
}

Vector3f Camera::getEyePoint() const
{
   return viewPosition_m;
}

void Camera::getFrame( const Scene& scene, const RandomMwc& random, Image& image ) const
{
   const RayTracer rayTracer( scene );

   const float width  = static_cast<float>(image.getWidth());
   const float height = static_cast<float>(image.getHeight());

   const float min_aa_steps = -2.0f;
   const float max_aa_steps = 2.0f;

   // do image sampling pixel loop
   for( float y = height;  y-- > 0.0f; )
   {
      for( float x = width;  x-- > 0.0f; )
      {
		  Vector3f radiance;

		  for ( float aa_x = min_aa_steps; aa_x < max_aa_steps; aa_x++)
		  {
			  for ( float aa_y = min_aa_steps; aa_y < max_aa_steps; aa_y++)
			  {
				 // make sample ray direction, stratified by pixels
				 Vector3f sampleDirection;
				 {
					// make image plane displacement vector coefficients
					const float xF = ((x + aa_x * 0.1f) * 2.0f / width ) - 1.0f;
					const float yF = ((y + aa_y * 0.1f) * 2.0f / height) - 1.0f;

					// make image plane offset vector
					const Vector3f offset( (right_m * xF) + (up_m * yF * (height / width)) );

					sampleDirection = (viewDirection_m + (offset * ::tanf(viewAngle_m * 0.5f))).unitize();
				 }

				 // get radiance from RayTracer
				 radiance = radiance + rayTracer.getRadiance( viewPosition_m, sampleDirection, random, 0 );
			  }
		  }

			radiance = radiance * (1.0f / 16.0f);

			// add radiance to image
			image.addToPixel( static_cast<dword>(x), static_cast<dword>(y), radiance );
		}
	  
		cout << static_cast<int>(static_cast<float>(y) / static_cast<float>(height) * 100) << "\r";
	}
}

/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/

#include <math.h>
#include <iostream>

#include "Image.hpp"

using namespace hxa7241_minilight;

namespace
{
   // format items
   const char PPM_ID[]        = "P6";
   const char MINILIGHT_URI[] = "http://www.hxa.name/minilight/";

   // guess of average screen maximum brightness
   const float DISPLAY_LUMINANCE_MAX = 700.0f;

   // ITU-R BT.709 standard RGB luminance weighting
   const Vector3f RGB_LUMINANCE( 0.2126f, 0.7152f, 0.0722f );

   // ITU-R BT.709 standard gamma
   const float GAMMA_ENCODE = 0.45f;
}

Image::Image
(
   std::istream& in
)
{
   // read width and height
   in >> width_m >> height_m;

   // condition width and height
   width_m  = width_m  < 1 ? 1 : (width_m  > 10000 ? 10000 : width_m );
   height_m = height_m < 1 ? 1 : (height_m > 10000 ? 10000 : height_m);

   pixels_m.resize( width_m * height_m );
}

void Image::addToPixel
(
   const dword     x,
   const dword     y,
   const Vector3f& radiance
)
{
   if( (x >= 0) & (x < width_m) & (y >= 0) & (y < height_m) )
   {
      const dword index = x + ((height_m - 1 - y) * width_m);
      pixels_m[ index ] = pixels_m[ index ] + radiance;
   }
}

dword Image::getWidth() const
{
   return width_m;
}

dword Image::getHeight() const
{
   return height_m;
}

void Image::getFormatted
(
   std::ostream& out,
   const dword   iteration
) const
{
   // make pixel value accumulation divider
   const float divider = 1.0f / static_cast<float>(
      (iteration > 0 ? iteration : 0) + 1);

   // write header
   {
      // write ID
      out << "#?RADIANCE" << '\n';

      // write other header things
      out << "FORMAT=32-bit_rgbe"  << '\n';
      out << "SOFTWARE=" << MINILIGHT_URI << '\n' << '\n';

      // write width, height
      out << "-Y " << height_m << " +X " << width_m << '\n';
   }

   // write pixels
   for( udword i = 0;  i < pixels_m.size();  ++i )
   {
      const udword rgbe = toRgbe( pixels_m[i] * divider );

      // write rgbe bytes
      for( dword b = 4;  b-- > 0; )
      {
         out << static_cast<ubyte>( (rgbe >> (b * 8)) & 0xFF );
      }
   }
}




/// implementation -------------------------------------------------------------
float Image::calculateToneMapping
(
   const std::vector<Vector3f>& pixels
)
{
   // calculate log mean luminance
   float logMeanLuminance = 1e-4f;
   {
      float sumOfLogs = 0.0f;
      for( int i = pixels.size();  i-- > 0; )
      {
         const float Y = pixels[i].dot( RGB_LUMINANCE );
         sumOfLogs += ::log10f( Y > 1e-4f ? Y : 1e-4f );
      }

      logMeanLuminance = ::powf( 10.0f, sumOfLogs / static_cast<float>(pixels.size()) );
   }

   // (what do these mean again? (must check the tech paper...))
   const float a = 1.219f + ::powf( DISPLAY_LUMINANCE_MAX * 0.25f, 0.4f );
   const float b = 1.219f + ::powf( logMeanLuminance, 0.4f );

   return ::powf( a / b, 2.5f ) / DISPLAY_LUMINANCE_MAX;
}

udword Image::toRgbe
(
   const Vector3f& rgbIn
)
{
   udword rgbe = 0;

   const Vector3f rgb( rgbIn.getClamped( Vector3f::ZERO(), Vector3f::MAX() ) );
   const float rgbLargest = (rgb[0] >= rgb[1]) ? (rgb[0] >= rgb[2] ? rgb[0] :
      rgb[2]) : (rgb[1] >= rgb[2] ? rgb[1] : rgb[2]);

   if( rgbLargest >= 1e-9f )
   {
      dword exponentLargest = 0;
      float mantissaLargest = ::frexpf( rgbLargest, &exponentLargest );

      // might be needed...
      //if( mantissaLargest == 1.0f )
      //{
      //   mantissaLargest = 0.5f;
      //   exponentLargest++;
      //}

      const float amount = mantissaLargest * 256.0f / rgbLargest;

      for( dword i = 3;  i-- > 0; )
      {
         rgbe |= static_cast<udword>(::floorf( rgb[i] * amount )) <<
            ((3 - i) * 8);
      }
      rgbe |= static_cast<udword>(exponentLargest + 128);
   }

   return rgbe;
}

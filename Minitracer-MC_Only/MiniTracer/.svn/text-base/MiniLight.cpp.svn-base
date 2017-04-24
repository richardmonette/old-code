/*------------------------------------------------------------------------------

   MiniLight C++ : minimal global illumination renderer
   Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.

   http://www.hxa.name/

------------------------------------------------------------------------------*/


#include <signal.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "Primitives.hpp"

#include "RandomMwc.hpp"
#include "Image.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Textures.h"




namespace
{

/// user messages --------------------------------------------------------------
const char BANNER_MESSAGE[] =
"\n  MiniLight 1.5.2.1 C++\n"
//"\n  MiniLight 1.5.2.1 C++ HDRI\n"   // HDRI
"  Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.\n"
"  http://www.hxa.name/minilight/\n"
"\n"
"  Further enhancements by Richard Monette\n"
"  Enhancements include: Interpolated normals using Barycentric co-odinates,"
"  Additional materials (Added perfect dielectric specular reflection and perfect dielectric refraction)"
"\n";

const char HELP_MESSAGE[] =
"\n"
"----------------------------------------------------------------------\n"
"  MiniLight 1.5.2.1 C++\n"
//"  MiniLight 1.5.2.1 C++ HDRI\n"   // HDRI
"\n"
"  Copyright (c) 2006-2009, Harrison Ainsworth / HXA7241.\n"
"  http://www.hxa.name/minilight/\n"
"\n"
"  2009-04-01\n"
"----------------------------------------------------------------------\n"
"\n"
"MiniLight is a minimal global illumination renderer.\n"
"\n"
"usage:\n"
"  minilight modelFilePathName\n"
"\n"
"The model text file format is:\n"
"  #MiniLight\n"
"\n"
"  iterations\n"
"\n"
"  imagewidth imageheight\n"
"\n"
"  viewposition viewdirection viewangle\n"
"\n"
"  skyemission groundreflection\n"
"  vertex0 vertex1 vertex2 reflectivity emitivity\n"
"  vertex0 vertex1 vertex2 reflectivity emitivity\n"
"  ...\n"
"\n"
"-- where iterations and image values are ints, viewangle is a float,\n"
"and all other values are three parenthised floats. The file must end\n"
"with a newline. Eg.:\n"
"  #MiniLight\n"
"\n"
"  100\n"
"\n"
"  200 150\n"
"\n"
"  (0 0.75 -2) (0 0 1) 45\n"
"\n"
"  (3626 5572 5802) (0.1 0.09 0.07)\n"
"  (0 0 0) (0 1 0) (1 1 0)  (0.7 0.7 0.7) (0 0 0)\n"
"\n";




/// ctrl-c handling ------------------------------------------------------------
const std::string*        pImageFilePathname_g = 0;
hxa7241_minilight::Image* pImage_g             = 0;
int                       frameNo_g            = 0;


void saveImage
(
   int arg
)
{
   // open image file
   std::ofstream imageFile( pImageFilePathname_g->c_str(), std::ios::binary );
   imageFile.exceptions( std::ios::badbit | std::ios::failbit |
      std::ios::eofbit );

   // write image frame to file
   pImage_g->getFormatted( imageFile, frameNo_g - 1 );

   imageFile.close();

   if( 0 != arg )
   {
      std::cout << "\ninterrupted\n";
      ::exit( EXIT_SUCCESS );
   }
}


const char   MODEL_FORMAT_ID[] = "#MiniLight";
const double SAVE_PERIOD       = 180.0;

}




/// entry point ////////////////////////////////////////////////////////////////
int main
(
   int   argc,
   char* argv[]
)
{
   int returnValue = EXIT_FAILURE;

   // catch everything
   try
   {
      // check for help request
      if( (argc <= 1) || (std::string(argv[1]) == "-?") ||
         (std::string(argv[1]) == "--help") )
      {
         std::cout << HELP_MESSAGE;
      }
      // execute
      else
      {
         std::cout << BANNER_MESSAGE;

         // get file names
         const std::string modelFilePathname( argv[1] );
         const std::string imageFilePathname( modelFilePathname + ".ppm" );
///*HDRI*/ const std::string imageFilePathname( modelFilePathname + ".rgbe" );

         // open model file
         std::ifstream modelFile( modelFilePathname.c_str() );
         modelFile.exceptions( std::ios::badbit | std::ios::eofbit );

         // check model file format identifier at start of first line
         {
            std::string formatId;
            modelFile >> formatId;
            if( std::string(MODEL_FORMAT_ID) != formatId )
            {
               throw std::runtime_error( "invalid model file" );
            }
         }

         // read frame iterations
         int iterations = 0;
         modelFile >> iterations;

		 //read the comma seperated list of textures in order
		 std::string texturesStr;
		 modelFile >> texturesStr;

		 Textures textures(texturesStr);

         // create top-level rendering objects with model file
               hxa7241_minilight::Image  image ( modelFile );
         const hxa7241_minilight::Camera camera( modelFile );
         const hxa7241_minilight::Scene  scene(  modelFile, camera.getEyePoint(), textures);

         modelFile.close();

         // setup ctrl-c/image-saving handler
         {
            pImageFilePathname_g = &imageFilePathname;
            pImage_g             = &image;

            ::signal( SIGINT, saveImage );
         }

         hxa7241_general::RandomMwc random;
         // make non-deterministic
         //hxa7241_general::RandomMwc random( static_cast<dword>(::time( 0 )) );

         // do progressive refinement render loop
         time_t lastTime = 0;
         for( int frameNo = 1;  frameNo <= iterations;  ++frameNo )
         {
            // render a frame
            camera.getFrame( scene, random, image );
            frameNo_g = frameNo;

            // save image every three minutes, and at start and end
            if( (SAVE_PERIOD < ::difftime( ::time( 0 ), lastTime )) |
               (frameNo == iterations) )
            {
               lastTime = ::time( 0 );

               saveImage( 0 );
            }

            // display latest frame number
            for( int b = frameNo > 1 ? static_cast<int>(::log10f(
               static_cast<float>(frameNo - 1))) : -1;
               b-- > -12;  std::cout << '\b' ) {}
            (std::cout << "iteration: " << frameNo).flush();
         }

         std::cout << "\nfinished\n";
      }

      returnValue = EXIT_SUCCESS;
   }
   // print exception message
   catch( const std::exception& e )
   {
      std::cout << '\n' << "*** execution failed:  " << e.what() << '\n';
   }
   catch( ... )
   {
      std::cout << '\n' << "*** execution failed" << '\n';
   }

   return returnValue;
}

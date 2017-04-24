
#include <signal.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "Primitives.hpp"

#include "PhotonMap.h"
#include "RandomMwc.hpp"
#include "Image.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Textures.h"

namespace
{

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

void main(int argc, char* argv[])
{
	// get file names
	const std::string modelFilePathname( argv[1] );
	const std::string imageFilePathname( modelFilePathname + ".hdr" );

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

	camera.getFrame( scene, random, image );
	
	saveImage( 0 );

	std::cout << "\nrendering complete...\n";
}

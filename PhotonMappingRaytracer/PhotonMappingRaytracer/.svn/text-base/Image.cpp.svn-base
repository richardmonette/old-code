#include "Image.h"
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

Image::Image(int w_, int h_ ) : w(w_), h(h_) 
{
    assert(w > 0 && h > 0);
    data = new rgb*[w];
    assert(data != 0);
    for (int i = 0; i < w; i++) {
        data[i] = new rgb[h];
        assert(data[i]);
    }
}

Image::Image(int w_, int h_ , rgb c) : w(w_), h(h_) 
{
    assert(w > 0 && h > 0);
    data = new rgb*[w];
    assert(data != 0);
    for (int i = 0; i < w; i++) {
        data[i] = new rgb[h];
        assert(data[i]);
        for (int j = 0; j < h; j++)
            data[i][j] = c;
    }
}

void Image::readPPM (char* file_name)
{
   int i, j;
   ifstream in;
   in.open(file_name);
   if (!in)
   {
      cerr << "ERROR -- Can't find PPM file  \'" << string(file_name) << "\'\n";
      exit(0);
   }
   string file_type, garbage;
   rgb pix_col;
   
   // read in ppm header info
   in >> file_type >> w >> h >> garbage;
   
   // create new raster with correct size 
   data = new rgb*[w];
   for (i = 0; i < w; i++)
      data[i] = new rgb[h];

   // now read in pixel rgb values and assign to raster
   if (file_type == "P3")
   {
      int red, green, blue;
      for (i = h-1; i >= 0; i--)
         for (j = 0; j < w; j++)
         {
            in >> red >> green >> blue;
            pix_col[0] = ((float(red)   + 0.5f) / 256.0f) * 
                         ((float(red)   + 0.5f) / 256.0f);
            pix_col[1] = ((float(green) + 0.5f) / 256.0f) * 
                         ((float(green) + 0.5f) / 256.0f);
            pix_col[2] = ((float(blue)  + 0.5f) / 256.0f) * 
                         ((float(blue)  + 0.5f) / 256.0f);

            data[j][i] = pix_col;
         }
   }
   else
   {
      char red, green, blue;
      in.get(red); // get rid of newline
      for (i = h-1; i >= 0; i--)
         for (j = 0; j < w; j++)
         {
            in.get(red);
            in.get(green);
            in.get(blue);
               
            pix_col[0] = ((float(red)   + 0.5f) / 256.0f) * 
                         ((float(red)   + 0.5f) / 256.0f);
            pix_col[1] = ((float(green) + 0.5f) / 256.0f) * 
                         ((float(green) + 0.5f) / 256.0f);
            pix_col[2] = ((float(blue)  + 0.5f) / 256.0f) * 
                         ((float(blue)  + 0.5f) / 256.0f);
      
            data[j][i] = pix_col;
         }
   }
}

void Image::Tonemap()
{
	const float Ywa = 10.0f;
	const float pre_scale = 1.0f;
	const float alpha = 0.1f;
	const float burn = 5.0f;
	const float post_scale = 1.0f;

	const float Yw = pre_scale * alpha * burn;
	const float invY2 = 1.f / (Yw * Yw);
	const float pScale = post_scale * pre_scale * alpha / Ywa;

	for (int y = h-1; y >= 0; y--) 
	for (int x = 0; x < w; x++) 
	{
		const float ys = data[x][y][1];
		data[x][y] *= (pScale * (1.f + ys * invY2) / (1.f + ys));
	}
}

void Image::writePPM(ostream& s) 
{
	Tonemap(); 

	s << "P6\n" << w << " " << h << "\n255\n";

	unsigned int i;
	for (int y = h-1; y >= 0; y--) 
	for (int x = 0; x < w; x++) 
	{
		float gamma = 1.0 / 2.2;

		data[x][y] = rgb( pow(data[x][y].r(), gamma),
						  pow(data[x][y].g(), gamma),
						  pow(data[x][y].b(), gamma)
						);

		i = (unsigned int)(256.0 * data[x][y].r());
		if (i > 255) i = 255;  
		s.put( (unsigned char) i );
		i = (unsigned int)(256.0 * data[x][y].g());
		if (i > 255) i = 255;  
		s.put( (unsigned char) i );
		i = (unsigned int)(256.0 * data[x][y].b());
		if (i > 255) i = 255;  
		s.put( (unsigned char) i );
	}
}



#pragma once

#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

struct tImage
{
	int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
	int sizeX;
	int sizeY;
	unsigned char *data;
};

tImage *LoadTGA(const char *strFileName);


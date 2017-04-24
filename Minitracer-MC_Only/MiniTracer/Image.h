#pragma once

#include <cmath>
#include <string>
#include <fstream>
#include "rgb.h"

using namespace std;

class Image
{
public:
	Image();
	Image(int width, int height);
	Image(int width, int height, Rgb background);
	~Image();

	bool set(int x, int y, const Rgb & color);
	void gammaCorrect(double gamma);
	void writePPM(ostream& out);
	void readPPM(string file_name);

private:

	Rgb** raster;
	int nx;
	int ny;
};

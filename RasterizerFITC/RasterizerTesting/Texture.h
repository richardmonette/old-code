#pragma once

#include "math.h"

class Texture
{
public:
	Texture();
	~Texture();

	void LoadTexture(const char *filename);

	static const int WIDTH = 512;
	static const int HEIGHT = 512;

	int *pixels;

};

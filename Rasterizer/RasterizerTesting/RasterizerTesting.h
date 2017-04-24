#pragma once

#include <vector>

using namespace std;

#include "Math3D.h"

const int resX = 640;
const int resY = 480;

struct  UV
{
	float u;
	float v;
};

struct Vertex
{
	Vec4	position;

	float invz;

	UV		diffuse_uv;
	UV		lightmap_uv;
};


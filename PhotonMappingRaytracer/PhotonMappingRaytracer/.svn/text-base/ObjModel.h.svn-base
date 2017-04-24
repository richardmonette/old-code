#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Material.h"
#include "DielectricMaterial.h"
#include "DiffSpecMaterial.h"
#include "PhongMetalMaterial.h"
#include "MirrorMaterial.h"
#include "DiffuseMaterial.h"
#include "SimpleTexture.h"
#include "LuminaireMaterial.h"

using namespace std;

struct ObjVertex 
{
	float x,y,z;
};

struct ObjUV
{
	float u,v;
};

struct ObjTriangle
{
	ObjVertex vertices[3];
	ObjUV uvs[3];
	ObjVertex normals[3];

	Material *mptr;

	bool isLight;
};

class ObjModel
{
public:
	ObjModel(const string filename);
	~ObjModel();

	int numTriangles;
	int numVertices;
	int numUVs;
	int numNormals;

	ObjVertex *vertices;
	ObjTriangle *triangles;
	ObjUV *uvs;
	ObjVertex *normals;

	float minX, minY, minZ, maxX, maxY, maxZ;

	SimpleTexture *greyTexture;
	DiffuseMaterial *greyMaterial;

	SimpleTexture *redTexture;
	DiffuseMaterial *redMaterial;

	SimpleTexture *blueTexture;
	DiffuseMaterial *blueMaterial;

	SimpleTexture *whiteTexture;
	DiffuseMaterial *whiteMaterial;

	SimpleTexture *tanTexture;
	DiffuseMaterial *tanMaterial;

	LuminaireMaterial *luminaireMaterial;

	MirrorMaterial *mirrorMaterial;

	DielectricMaterial *dielectricMaterial;

};


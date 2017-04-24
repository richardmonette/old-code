#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Triangle.h"

using namespace std;

class ObjModel
{

public:

	ObjModel(const string filename);
	~ObjModel();

	int numTriangles;
	int numVertices;
	int numUVs;
	int numNormals;

	Vector3 *vertices;
	Triangle *triangles;
	Vector2 *uvs;
	Vector3 *normals;

	float minX, minY, minZ, maxX, maxY, maxZ;

};


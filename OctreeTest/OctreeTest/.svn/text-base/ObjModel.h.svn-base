#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

using namespace std;

struct Triangle
{
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	Vector3 n0;
	Vector3 n1;
	Vector3 n2;
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

	Vector3 *vertices;
	Triangle *triangles;
	Vector2 *uvs;
	Vector3 *normals;

	float minX, minY, minZ, maxX, maxY, maxZ;

};


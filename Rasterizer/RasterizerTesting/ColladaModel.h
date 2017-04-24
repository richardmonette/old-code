#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Texture.h"
#include "RasterizerTesting.h"

using namespace std;

class Renderer;

class ColladaModel
{

public:
	ColladaModel(const char *filename, Renderer *renderer);
	~ColladaModel();

	int GetNumIndices();
	Triangle * GetTriangle(int i);
	Vertex * GetVertex(int i);
	UV * GetUV(int i);

	int GetDiffuse();

private:

	//Index for the texture (which is stored in the renderer)
	int diffuse;
	int normal;

	int nVertices;
	int nIndices;

	int nUVs;
	int nUVIndices;

	int nNormals;
	int nNormalIndices;

	int nTangents;
	int nTangentIndices;

	int nBinormals;
	int nBinormalIndices;

	Triangle	tris[20000];
	
	UV			uvs[20000];
	
	Vertex		vertices[20000];
	Vertex 		normals[20000];
	Vertex 		tangents[20000];
	Vertex		binormals[20000];

};

void StringExplode(string str, string separator, vector<string>* results);

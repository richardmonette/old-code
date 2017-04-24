
#include "ObjModel.h"

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;

	found = str.find_first_of(separator);

	while(found != string::npos)
	{
		if(found > 0)
		{
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}

	if(str.length() > 0)
	{
		results->push_back(str);
	}
}

ObjModel::ObjModel(const string filename)
{
	minX = 999999;
	minY = 999999;
	minZ = 999999;
	
	maxX = -999999;
	maxY = -999999;
	maxZ = -999999;

	bool lightMode = false;
	int currentMaterial = 0;
	Vector3 currentColor;

	vertices = new Vector3[3000000];
	uvs = new Vector2[3000000];
	normals = new Vector3[3000000];
	triangles = new Triangle[3000000];

	numVertices = 0;
	numTriangles = 0;
	numUVs = 0;
	numNormals = 0;

	FILE *file = fopen ( filename.c_str(), "r" );

	if ( file != NULL )
	{
		char line[256];

		while ( fgets( line, sizeof line, file ) != NULL )
		{
			vector<string> shrapnel;

			StringExplode(line, " ", &shrapnel);

			if (strcmp(shrapnel[0].c_str(), "v") == 0)
			{
				vertices[numVertices].setX(atof(shrapnel[1].c_str()));
				vertices[numVertices].setY(atof(shrapnel[2].c_str()));
				vertices[numVertices].setZ(atof(shrapnel[3].c_str()));

				if ( vertices[numVertices].x() < minX ) minX = vertices[numVertices].x();
				if ( vertices[numVertices].y() < minY ) minY = vertices[numVertices].y();
				if ( vertices[numVertices].z() < minZ ) minZ = vertices[numVertices].z();

				if ( vertices[numVertices].x() > maxX ) maxX = vertices[numVertices].x();
				if ( vertices[numVertices].y() > maxY ) maxY = vertices[numVertices].y();
				if ( vertices[numVertices].z() > maxZ ) maxZ = vertices[numVertices].z();

				numVertices++;
			}
			else if (strcmp(shrapnel[0].c_str(), "vn") == 0)
			{
				normals[numNormals].setX(atof(shrapnel[1].c_str()));
				normals[numNormals].setY(atof(shrapnel[2].c_str()));
				normals[numNormals].setZ(atof(shrapnel[3].c_str()));

				numNormals++;
			}
			else if (strcmp(shrapnel[0].c_str(), "vt") == 0)
			{
				uvs[numUVs].setX(atof(shrapnel[1].c_str()));
				uvs[numUVs].setY(atof(shrapnel[2].c_str()));

				numUVs++;
			}
			else if (strcmp(shrapnel[0].c_str(), "f") == 0)
			{
				vector<string> vert0, vert1, vert2;
				StringExplode(shrapnel[1], "/", &vert0);
				StringExplode(shrapnel[2], "/", &vert1);
				StringExplode(shrapnel[3], "/", &vert2);

				triangles[numTriangles].p0 = vertices[atoi(vert0[0].c_str())-1];
				triangles[numTriangles].p1 = vertices[atoi(vert1[0].c_str())-1];
				triangles[numTriangles].p2 = vertices[atoi(vert2[0].c_str())-1];

				triangles[numTriangles].n0 = normals[atoi(vert0[2].c_str())-1];
				triangles[numTriangles].n1 = normals[atoi(vert1[2].c_str())-1];
				triangles[numTriangles].n2 = normals[atoi(vert2[2].c_str())-1];

				//triangles[numTriangles].u0 = uvs[atoi(vert0[1].c_str())-1];
				//triangles[numTriangles].u1 = uvs[atoi(vert1[1].c_str())-1];
				//triangles[numTriangles].u2 = uvs[atoi(vert2[1].c_str())-1];

				//triangles[numTriangles].material = currentMaterial;
				//triangles[numTriangles].color = currentColor;

				numTriangles++;
			}
			else if (strcmp(shrapnel[0].c_str(), "usemtl") == 0)
			{
				shrapnel[1] = shrapnel[1].substr(0, shrapnel[1].size()-1);

				if (shrapnel[1].compare("lambert1SG") == 0)
				{
					currentMaterial = 0;
					lightMode = false;
					currentColor = Vector3(0.75,0.75,0.75);
				}
				else if (shrapnel[1].compare("lambert2SG") == 0)
				{
					currentMaterial = 0;
					lightMode = false;
					currentColor = Vector3(0.75,0.25,0.25);
				}
				else if (shrapnel[1].compare("lambert3SG") == 0)
				{
					currentMaterial = 0;
					lightMode = false;
					currentColor = Vector3(0.25,0.25,0.75);
				}
				else if (shrapnel[1].compare("lambert4SG") == 0)
				{
					currentMaterial = 0;
					lightMode = true;
					currentColor = Vector3(0.75,0.75,0.75);
				}
				else if (shrapnel[1].compare("phong1SG") == 0)
				{
					currentMaterial = 1;
					lightMode = false;
					currentColor = Vector3(0.9,0.9,0.9);
				}
				else if (shrapnel[1].compare("phong2SG") == 0)
				{
					currentMaterial = 2;
					lightMode = false;
					currentColor = Vector3(0.9,0.9,0.9);
				}
				else
				{
					currentMaterial = 0;
					lightMode = false;
					currentColor = Vector3(0.75,0.75,0.75);
				}
			}	
		}
	}
	else
	{
		cout << "COULD NOT LOAD MODEL\n";
	}

	cout << filename << " loaded... " << numVertices << " vertices " << numTriangles << " triangles " << numNormals << " normals \n";
}

ObjModel::~ObjModel()
{
	delete vertices;
	delete triangles;
	delete uvs;
	delete normals;
}


#include "ObjModel.h"
#include "Util.h"

ObjModel::ObjModel(const string filename)
{
	greyTexture = new SimpleTexture(rgb(0.7f,0.7f,0.7f));
	greyMaterial = new DiffuseMaterial(greyTexture);

	redTexture = new SimpleTexture(rgb(0.75f,0.25f,0.25f));
	redMaterial = new DiffuseMaterial(redTexture);

	blueTexture = new SimpleTexture(rgb(0.25f,0.25f,0.75f));
	blueMaterial = new DiffuseMaterial(blueTexture);

	tanTexture = new SimpleTexture(rgb(214.f / 256.f, 194.f/256.f, 162.f/256.f));
	tanMaterial = new DiffuseMaterial(tanTexture);

	whiteTexture = new SimpleTexture(rgb(1000.0f,1000.0f,1000.0f));
	whiteMaterial = new DiffuseMaterial(whiteTexture);

	luminaireMaterial = new LuminaireMaterial(whiteTexture, 1.0, 0.0, whiteMaterial, NULL, NULL);

	mirrorMaterial = new MirrorMaterial();

	dielectricMaterial = new DielectricMaterial(1.7f, rgb(1,1,1));

	minX = 999999;
	minY = 999999;
	minZ = 999999;
	
	maxX = -999999;
	maxY = -999999;
	maxZ = -999999;

	Material *currentMaterial = greyMaterial;
	bool lightMode = false;

	vertices = new ObjVertex[300000];
	triangles = new ObjTriangle[300000];
	uvs = new ObjUV[300000];
	normals = new ObjVertex[300000];

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
				vertices[numVertices].x = atof(shrapnel[1].c_str());
				vertices[numVertices].y = atof(shrapnel[2].c_str());
				vertices[numVertices].z = atof(shrapnel[3].c_str());

				if ( vertices[numVertices].x < minX ) minX = vertices[numVertices].x;
				if ( vertices[numVertices].y < minY ) minY = vertices[numVertices].y;
				if ( vertices[numVertices].z < minZ ) minZ = vertices[numVertices].z;

				if ( vertices[numVertices].x > maxX ) maxX = vertices[numVertices].x;
				if ( vertices[numVertices].y > maxY ) maxY = vertices[numVertices].y;
				if ( vertices[numVertices].z > maxZ ) maxZ = vertices[numVertices].z;

				numVertices++;
			}
			else if (strcmp(shrapnel[0].c_str(), "vn") == 0)
			{
				normals[numNormals].x = atof(shrapnel[1].c_str());
				normals[numNormals].y = atof(shrapnel[2].c_str());
				normals[numNormals].z = atof(shrapnel[3].c_str());

				numNormals++;
			}
			else if (strcmp(shrapnel[0].c_str(), "vt") == 0)
			{
				uvs[numUVs].u = atof(shrapnel[1].c_str());
				uvs[numUVs].v = atof(shrapnel[2].c_str());

				numUVs++;
			}
			else if (strcmp(shrapnel[0].c_str(), "f") == 0)
			{
				vector<string> vert0, vert1, vert2;
				StringExplode(shrapnel[1], "/", &vert0);
				StringExplode(shrapnel[2], "/", &vert1);
				StringExplode(shrapnel[3], "/", &vert2);

				triangles[numTriangles].vertices[0] = vertices[atoi(vert0[0].c_str())-1];
				triangles[numTriangles].vertices[1] = vertices[atoi(vert1[0].c_str())-1];
				triangles[numTriangles].vertices[2] = vertices[atoi(vert2[0].c_str())-1];

				triangles[numTriangles].normals[0] = normals[atoi(vert0[2].c_str())-1];
				triangles[numTriangles].normals[1] = normals[atoi(vert1[2].c_str())-1];
				triangles[numTriangles].normals[2] = normals[atoi(vert2[2].c_str())-1];

				triangles[numTriangles].uvs[0] = uvs[atoi(vert0[1].c_str())-1];
				triangles[numTriangles].uvs[1] = uvs[atoi(vert1[1].c_str())-1];
				triangles[numTriangles].uvs[2] = uvs[atoi(vert2[1].c_str())-1];

				triangles[numTriangles].isLight = lightMode;
				triangles[numTriangles].mptr = currentMaterial;

				numTriangles++;
			}
			else if (strcmp(shrapnel[0].c_str(), "usemtl") == 0)
			{
				shrapnel[1] = shrapnel[1].substr(0, shrapnel[1].size()-1);

				if (shrapnel[1].compare("lambert5SG") == 0)
				{
					currentMaterial = greyMaterial;
					lightMode = false;
				}
				else if (shrapnel[1].compare("lambert3SG") == 0)
				{
					currentMaterial = blueMaterial;
					lightMode = false;
				}
				else if (shrapnel[1].compare("lambert2SG") == 0)
				{
					currentMaterial = redMaterial;
					lightMode = false;
				}
				else if (shrapnel[1].compare("lambert4SG") == 0)
				{
					currentMaterial = luminaireMaterial;
					lightMode = true;
				}
				else if (shrapnel[1].compare("phong1SG") == 0)
				{
					currentMaterial = mirrorMaterial;
					lightMode = false;
				}
				else if (shrapnel[1].compare("phong2SG") == 0)
				{
					currentMaterial = dielectricMaterial;
					lightMode = false;
				}
			}	
		}
	}

	cout << filename << " loaded... " << numVertices << " vertices " << numTriangles << " triangles" << "\n";
}

ObjModel::~ObjModel()
{
	delete vertices;
	delete triangles;
}

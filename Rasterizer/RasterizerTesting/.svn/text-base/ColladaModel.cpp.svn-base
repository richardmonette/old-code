#include "rapidxml/rapidxml.hpp"

#include "Renderer.h"
#include "ColladaModel.h"

using namespace rapidxml;
using namespace std;

static const char * modelPath = "C:\\Documents and Settings\\Administrator\\My Documents\\Rasterizer\\Content\\labscene.dae";
static const char * texturePath = "C:\\Documents and Settings\\Administrator\\My Documents\\Rasterizer\\Content\\labscene.png";

//static const char * modelPath = "C:\\Documents and Settings\\Administrator\\My Documents\\Visual Studio 2008\\Projects\\RasterizerTesting\\Content\\labscene.dae";
//static const char * texturePath = "C:\\Documents and Settings\\Administrator\\My Documents\\Visual Studio 2008\\Projects\\RasterizerTesting\\Content\\labscene.png";

ColladaModel::ColladaModel(const char *filename, Renderer *renderer)
{
	nVertices = 0;
	nIndices = 0;

	nUVs = 0;
	nUVIndices = 0;

	nNormals = 0;
	nNormalIndices = 0;

	nTangents = 0;
	nTangentIndices = 0;

	nBinormals = 0;
	nBinormalIndices = 0;

	char xml[400000];
	char temp[300000];

	ifstream inf( modelPath );

	strcpy(xml, "");

	while( inf.getline(temp, 300000) )
	{
		strncat (xml, temp, 300000);
	}

	strcat(xml, "\0");

	inf.close();

	xml_document<> doc;
	doc.parse<0>(xml);

	xml_node<> *node = doc.first_node();
	xml_node<> *library_geometries = node->first_node("library_geometries");
	xml_node<> *geometry = library_geometries->first_node("geometry");
	xml_node<> *mesh = geometry->first_node("mesh");
	xml_node<> *source = mesh->first_node("source");
	
	xml_node<> *verticesNode = source->first_node("float_array");
	
	xml_node<> *normalsNode = source->next_sibling()->first_node("float_array");

	xml_node<> *uvsNode = source->next_sibling()->next_sibling()->first_node("float_array");
	
	xml_node<> *indicesNode = source->next_sibling()->next_sibling()->next_sibling()->next_sibling()->first_node("p");

	vector<string> verticesVector;
	string vertString = verticesNode->value();
	StringExplode(vertString, " ", &verticesVector);

	float x, y, z;
	int i = 0;

	for (unsigned int ct = 0; ct < verticesVector.size(); ct+=3)
	{
		x = (float)atof( verticesVector[ct].c_str() );
		y = (float)atof( verticesVector[ct + 1].c_str() );
		z = (float)atof( verticesVector[ct + 2].c_str() );

		vertices[i].coordsLocal.x = x;
		vertices[i].coordsLocal.y = y;
		vertices[i].coordsLocal.z = z;
		vertices[i].coordsLocal.w = 1.0f;

		i++;
	}

	vector<string> uvsVector;
	string uvsString = uvsNode->value();
	StringExplode(uvsString, " ", &uvsVector);

	int r = 0;

	for (unsigned int uvCT = 0; uvCT < uvsVector.size(); uvCT+=2)
	{
		uvs[r].u = (float)atof( uvsVector[uvCT].c_str() );
		uvs[r].v = (float)atof( uvsVector[uvCT + 1].c_str() );

		uvs[r].u *= 256;
		uvs[r].v *= 256;

		uvs[r].v = 256 - uvs[r].v;

		r++;
	}

	vector<string> normalsVector;
	string normalsString = normalsNode->value();
	StringExplode(normalsString, " ", &normalsVector);

	int normalCT = 0;

	for (int j = 0; j < nNormals; j++ )
	{
		normals[j].coordsLocal.x = (float)atof( normalsVector[normalCT].c_str() );
		normals[j].coordsLocal.y = (float)atof( normalsVector[normalCT + 1].c_str() );
		normals[j].coordsLocal.z = (float)atof( normalsVector[normalCT + 2].c_str() );

		normalCT += 3;
	}

	/*vector<string> tangentsVector;
	string tangentsString = tangentsNode->value();
	StringExplode(tangentsString, " ", &tangentsVector);

	int tangentCT = 0;

	for (int v = 0; v < nTangents; v++)
	{
		tangents[v].coordsLocal.x = (float)atof( tangentStream[tangentCT] );
		tangents[v].coordsLocal.y = (float)atof( tangentStream[tangentCT + 1];
		tangents[v].coordsLocal.z = (float)atof( tangentStream[tangentCT + 2];

		tangentCT += 3;
	}

	int binormalCT = 0;

	for (int c = 0; c < nBinormals; c++)
	{
		binormals[c].coordsLocal.x = (float)atof( binormalStream[binormalCT];
		binormals[c].coordsLocal.y = (float)atof( binormalStream[binormalCT + 1];
		binormals[c].coordsLocal.z = (float)atof( binormalStream[binormalCT + 2];

		binormalCT += 3;
	}*/

	vector<string> indexVector;
	string indexString = indicesNode->value();
	StringExplode(indexString, " ", &indexVector);

	int indexarr[20000];
	int uvarr[20000];
	int normalarr[20000];

	int count = 0;
	for (unsigned int i = 0; i < indexVector.size(); i+=3)
	{
		indexarr[count] = atoi(indexVector[i+0].c_str());
		normalarr[count] = atoi(indexVector[i+1].c_str());
		uvarr[count] = atoi(indexVector[i+2].c_str());

		count++;
	}

	for (int j = 0; j < count/3; j++)
	{		
		tris[j].vertices[0] = vertices[(int)indexarr[j * 3]];//tris[j].a = (int)indexarr[j * 3];
		tris[j].vertices[1] = vertices[(int)indexarr[j * 3 + 1]];//tris[j].b = (int)indexarr[j * 3 + 1];
		tris[j].vertices[2] = vertices[(int)indexarr[j * 3 + 2]];//tris[j].c = (int)indexarr[j * 3 + 2];

		tris[j].uva = (int)uvarr[j * 3];
		tris[j].uvb = (int)uvarr[j * 3 + 1];
		tris[j].uvc = (int)uvarr[j * 3 + 2];

		tris[j].vertices[0].uv = uvs[tris[j].uva];
		tris[j].vertices[1].uv = uvs[tris[j].uvb];
		tris[j].vertices[2].uv = uvs[tris[j].uvc];

		tris[j].normala = (int)normalarr[j * 3];
		tris[j].normalb = (int)normalarr[j * 3 + 1];
		tris[j].normalc = (int)normalarr[j * 3 + 2];

		/*tris[j].tangenta = (int)tangentIndexStream[j * 3];
		tris[j].tangentb = (int)tangentIndexStream[j * 3 + 1];
		tris[j].tangentc = (int)tangentIndexStream[j * 3 + 2];

		tris[j].binormala = (int)binormalIndexStream[j * 3];
		tris[j].binormalb = (int)binormalIndexStream[j * 3 + 1];
		tris[j].binormalc = (int)binormalIndexStream[j * 3 + 2];*/

	}

	nIndices = count / 3;

	//diffuse = new Texture( texturePath );
	diffuse = renderer->CreateTexture(texturePath);
}

int ColladaModel::GetDiffuse()
{
	return diffuse;
}

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos){
		if(found > 0){
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0){
		results->push_back(str);
	}
}

int ColladaModel::GetNumIndices()
{
	return nIndices;
}

Triangle * ColladaModel::GetTriangle(int i)
{
	return &tris[i];
}

Vertex * ColladaModel::GetVertex(int i)
{
	return &vertices[i];
}

UV * ColladaModel::GetUV(int i)
{
	return &uvs[i];
}



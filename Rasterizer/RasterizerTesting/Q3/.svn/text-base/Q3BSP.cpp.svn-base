#include "Q3Camera.h"
#include "Q3BSP.h"
#include "Q3Frustum.h"

#define MAX_PATH 128

Q3Frustum g_Frustum;

extern bool g_bLightmaps;
extern bool g_bTextures;

Q3BSP::Q3BSP()
{
	m_numOfVerts     = 0;	
	m_numOfFaces     = 0;
	m_numOfIndices   = 0;
	m_numOfTextures  = 0;
	m_numOfLightmaps = 0;

	m_numOfNodes	 = 0;
	m_numOfLeafs	 = 0;
	m_numOfLeafFaces = 0;
	m_numOfPlanes	 = 0;

	m_pVerts		 = NULL;	
	m_pFaces		 = NULL;
	m_pIndices       = NULL;
	
	m_pNodes		 = NULL;
	m_pLeafs		 = NULL;
	m_pPlanes		 = NULL;
	m_pLeafFaces	 = NULL;
	memset(&m_clusters, 0, sizeof(tBSPVisData));

	vertices = new Vertex[16];
}

bool Q3BSP::LoadBSP(const char *strFileName, Renderer *renderer)
{
	FILE *fp = NULL;
	int i = 0;
	
	if((fp = fopen(strFileName, "rb")) == NULL)
	{
		return false;
	}

	tBSPHeader header = {0};
	tBSPLump lumps[kMaxLumps] = {0};

	fread(&header, 1, sizeof(tBSPHeader), fp);
	fread(&lumps, kMaxLumps, sizeof(tBSPLump), fp);

	m_numOfVerts = lumps[kVertices].length / sizeof(tBSPVertex);
	m_pVerts     = new tBSPVertex [m_numOfVerts];

	m_numOfFaces = lumps[kFaces].length / sizeof(tBSPFace);
	m_pFaces     = new tBSPFace [m_numOfFaces];

	m_numOfIndices = lumps[kIndices].length / sizeof(int);
	m_pIndices     = new int [m_numOfIndices];

	m_numOfTextures = lumps[kTextures].length / sizeof(tBSPTexture);
	tBSPTexture *pTextures = new tBSPTexture [m_numOfTextures];
 
	m_numOfLightmaps = lumps[kLightmaps].length / sizeof(tBSPLightmap);
	tBSPLightmap *pLightmaps = new tBSPLightmap [m_numOfLightmaps ];

	fseek(fp, lumps[kVertices].offset, SEEK_SET);

	for(i = 0; i < m_numOfVerts; i++)
	{
		fread(&m_pVerts[i], 1, sizeof(tBSPVertex), fp);
		
		float temp = m_pVerts[i].vPosition.y;
		m_pVerts[i].vPosition.y = m_pVerts[i].vPosition.z;
		m_pVerts[i].vPosition.z = -temp;
	}	

	fseek(fp, lumps[kIndices].offset, SEEK_SET);

	fread(m_pIndices, m_numOfIndices, sizeof(int), fp);

	fseek(fp, lumps[kFaces].offset, SEEK_SET);

	fread(m_pFaces, m_numOfFaces, sizeof(tBSPFace), fp);

	fseek(fp, lumps[kTextures].offset, SEEK_SET);
	
	fread(pTextures, m_numOfTextures, sizeof(tBSPTexture), fp);

	for(i = 0; i < m_numOfTextures; i++)
	{
		//Hacking in some logic here to handle the filepaths
		char filename[128] = "../Content/";
		//char filename[256] = "C:/Documents and Settings/Administrator/My Documents/Visual Studio 2008/Projects/RasterizerTesting/Content/";

		strcat(filename, pTextures[i].strName);
		strcat(filename, ".tga");

		m_textures[i] = renderer->CreateTexture(filename);
	}

	delete [] pTextures;

	fseek(fp, lumps[kLightmaps].offset, SEEK_SET);

	for(i = 0; i < m_numOfLightmaps ; i++)
	{
		fread(&pLightmaps[i], 1, sizeof(tBSPLightmap), fp);
		renderer->CopyLightmap(&pLightmaps[i]);
	}

	//delete [] pLightmaps;

	m_numOfNodes = lumps[kNodes].length / sizeof(tBSPNode);
	m_pNodes     = new tBSPNode [m_numOfNodes];

	fseek(fp, lumps[kNodes].offset, SEEK_SET);
	fread(m_pNodes, m_numOfNodes, sizeof(tBSPNode), fp);

	m_numOfLeafs = lumps[kLeafs].length / sizeof(tBSPLeaf);
	m_pLeafs     = new tBSPLeaf [m_numOfLeafs];

	fseek(fp, lumps[kLeafs].offset, SEEK_SET);
	fread(m_pLeafs, m_numOfLeafs, sizeof(tBSPLeaf), fp);

	for(i = 0; i < m_numOfLeafs; i++)
	{
		int temp = m_pLeafs[i].min.y;
		m_pLeafs[i].min.y = m_pLeafs[i].min.z;
		m_pLeafs[i].min.z = -temp;

		temp = m_pLeafs[i].max.y;
		m_pLeafs[i].max.y = m_pLeafs[i].max.z;
		m_pLeafs[i].max.z = -temp;
	}

	m_numOfLeafFaces = lumps[kLeafFaces].length / sizeof(int);
	m_pLeafFaces     = new int [m_numOfLeafFaces];

	fseek(fp, lumps[kLeafFaces].offset, SEEK_SET);
	fread(m_pLeafFaces, m_numOfLeafFaces, sizeof(int), fp);

	m_numOfPlanes = lumps[kPlanes].length / sizeof(tBSPPlane);
	m_pPlanes     = new tBSPPlane [m_numOfPlanes];

	fseek(fp, lumps[kPlanes].offset, SEEK_SET);
	fread(m_pPlanes, m_numOfPlanes, sizeof(tBSPPlane), fp);

	for(i = 0; i < m_numOfPlanes; i++)
	{
		float temp = m_pPlanes[i].vNormal.y;
		m_pPlanes[i].vNormal.y = m_pPlanes[i].vNormal.z;
		m_pPlanes[i].vNormal.z = -temp;
	}

	fseek(fp, lumps[kVisData].offset, SEEK_SET);

	if(lumps[kVisData].length) 
	{
		fread(&(m_clusters.numOfClusters),	 1, sizeof(int), fp);
		fread(&(m_clusters.bytesPerCluster), 1, sizeof(int), fp);

		int size = m_clusters.numOfClusters * m_clusters.bytesPerCluster;
		m_clusters.pBitsets = new char[size];

		fread(m_clusters.pBitsets, 1, sizeof(char) * size, fp);
	}
	else
		m_clusters.pBitsets = NULL;


	fclose(fp);

	m_FacesDrawn.Resize(m_numOfFaces);

	return true;
}

int Q3BSP::FindLeaf(const Vec3 *position)
{
	int i = 0;
	float distance = 0.0f;

	while(i >= 0)
	{
		const tBSPNode&  node = m_pNodes[i];
		const tBSPPlane& plane = m_pPlanes[node.plane];

        distance =	plane.vNormal.x * position->x + 
					plane.vNormal.y * position->y + 
					plane.vNormal.z * position->z - plane.d;

        if(distance >= 0)	
		{
            i = node.front;
        }
        else		
		{
            i = node.back;
        }
    }

    return ~i;
}

inline int Q3BSP::IsClusterVisible(int current, int test)
{
	if(!m_clusters.pBitsets || current < 0) return 1;

	char visSet = m_clusters.pBitsets[(current*m_clusters.bytesPerCluster) + (test / 8)];
	
	int result = visSet & (1 << ((test) & 7));

	return ( result );
}

void Q3BSP::RenderFace(int faceIndex, Renderer *renderer)
{
	tBSPFace *pFace = &m_pFaces[faceIndex];
	
	renderer->SetDiffuseTexture(pFace->textureID);
	renderer->SetLightmapTexture(pFace->lightmapID);

	renderer->DrawElements(pFace->numOfIndices, &m_pIndices[pFace->startIndex], &m_pVerts[pFace->startVertIndex]);
}

void Q3BSP::RenderLevel(Vec3 *position, Renderer *renderer, Mat4x4 *comboMatrix)
{
	g_Frustum.CalculateFrustum(comboMatrix, true);

	m_FacesDrawn.ClearAll();

	const int leafIndex = FindLeaf(position);

	const int cluster = m_pLeafs[leafIndex].cluster;

	int i = m_numOfLeafs;

	while(i--)
	{
		tBSPLeaf *pLeaf = &(m_pLeafs[i]);

		if(!IsClusterVisible(cluster, pLeaf->cluster)) 
		{
			continue;
		}

		if(!g_Frustum.BoxInFrustum((float)pLeaf->min.x, (float)pLeaf->min.y, (float)pLeaf->min.z, (float)pLeaf->max.x, (float)pLeaf->max.y, (float)pLeaf->max.z))
		{
			//continue;
		}
		
		int faceCount = pLeaf->numOfLeafFaces;

		while(faceCount--)
		{
			int faceIndex = m_pLeafFaces[pLeaf->leafface + faceCount];

			if(!m_FacesDrawn.On(faceIndex)) 
			{
				m_FacesDrawn.Set(faceIndex);
				RenderFace(faceIndex, renderer);
			}
		}			
	}
}

void Q3BSP::Destroy()
{
	if(m_pVerts) 
	{
		delete [] m_pVerts;		m_pVerts = NULL;
	}

	if(m_pFaces)	
	{
		delete [] m_pFaces;		m_pFaces = NULL;
	}

	if(m_pIndices)	
	{
		delete [] m_pIndices;
		m_pIndices = NULL;
	}

	if(m_pNodes)	
	{
		delete [] m_pNodes;		m_pNodes = NULL;
	}

	if(m_pLeafs)	
	{
		delete [] m_pLeafs;		m_pLeafs = NULL;
	}

	if(m_pLeafFaces)	
	{
		delete [] m_pLeafFaces;	m_pLeafFaces = NULL;
	}

	if(m_pPlanes)	
	{
		delete [] m_pPlanes;	m_pPlanes = NULL;
	}

	if(m_clusters.pBitsets)	
	{
		delete [] m_clusters.pBitsets;		m_clusters.pBitsets = NULL;
	}
}

Q3BSP::~Q3BSP()
{
	Destroy();
}


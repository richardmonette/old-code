#pragma once

#include "EntityManager.h"
#include "Physics/PhysicsManager.h"
#include "DefaultShader.h"

#include "GL/glew.h"
#include "freeglut.h"

class Frustum;



struct tVector3i
{
	int x, y, z;
};

struct tBSPHeader
{
    char strID[4];
    int version;
}; 

struct tBSPLump
{
	int offset;
	int length;
};

struct tBSPFace
{
    int textureID;
    int effect;	
    int type;
    int startVertIndex;
    int numOfVerts;
    int startIndex;	
    int numOfIndices;
    int lightmapID;	
    int lMapCorner[2];
    int lMapSize[2];
    Vector3 lMapPos;
    Vector3 lMapVecs[2];
    Vector3 vNormal;
    int size[2];
};

struct tBSPTexture
{
    char strName[64];
    int flags;
    int textureType;
};

struct tBSPLightmap
{
    unsigned char imageBits[128][128][3];
};

struct tBSPNode
{
    int plane;
    int front;
    int back;
    tVector3i min;
    tVector3i max;
}; 

struct tBSPLeaf
{
    int cluster;
    int area;
    tVector3i min;
    tVector3i max;
    int leafface;
    int numOfLeafFaces;
    int leafBrush; 
    int numOfLeafBrushes;
}; 

struct tBSPPlane
{
    Vector3 vNormal;
    float d;
};

struct tBSPVisData
{
	int numOfClusters;
	int bytesPerCluster;
	unsigned char *pBitsets;				
};

struct tBSPBrush 
{
	int brushSide;
	int numOfBrushSides;
	int textureID;
}; 

struct tBSPBrushSide 
{
	int plane;
	int textureID;
}; 

enum eLumps
{
    kEntities = 0,
    kTextures,
    kPlanes,
    kNodes,
    kLeafs,
    kLeafFaces,
    kLeafBrushes,
    kModels,
    kBrushes,
    kBrushSides,
    kVertices,
    kIndices,
    kShaders,
    kFaces,
    kLightmaps,
    kLightVolumes,
    kVisData,
    kMaxLumps
};

class CBitset 
{

public:

    CBitset() : m_bits(0), m_size(0) {}

	~CBitset() 
	{
		if(m_bits) 
		{
			delete m_bits;
			m_bits = NULL;
		}
	}

	void Resize(int count) 
	{ 
		m_size = count/32 + 1;

        if(m_bits) 
		{
			delete m_bits;
			m_bits = 0;
		}

		m_bits = new unsigned int[m_size];
		ClearAll();
	}

	void Set(int i) 
	{
		m_bits[i >> 5] |= (1 << (i & 31));
	}

	int On(int i) 
	{
		return m_bits[i >> 5] & (1 << (i & 31 ));
	}

	void Clear(int i) 
	{
		m_bits[i >> 5] &= ~(1 << (i & 31));
	}

	void ClearAll() 
	{
		memset(m_bits, 0, sizeof(unsigned int) * m_size);
	}

private:

	unsigned int *m_bits;
	int m_size;
};

class Q3BSP
{

public:

	Q3BSP();
	~Q3BSP();

	bool LoadBSP(const char *strFileName);
	void LoadModels(tBSPLump *lumps, FILE *fp); // Since this is such a complicated process it is getting its own function
	
	void RenderLevel(const Vector3 &vPos, DefaultShader *shader, Frustum *frustum);

	void RenderModel(tModel *model, DefaultShader *shader);

	void Update(float milliseconds);

	void Destroy();

	PhysicsManager physicsManager;

private:

	void ChangeGamma(unsigned char *pImage, int size, float factor);
	void CreateLightmapTexture(GLuint &texture, unsigned char *pImageBits, int width, int height);
	int IsClusterVisible(int current, int test);
	int FindLeaf(const Vector3 &vPos);
	
	void RenderFace(int faceIndex, DefaultShader *shader);

	int m_numOfVerts;			// The number of verts in the model
	int m_numOfFaces;			// The number of faces in the model
	int m_numOfIndices;			// The number of indices for the model
	int m_numOfTextures;		// The number of texture maps
	int m_numOfLightmaps;		// The number of light maps
	int m_numOfNodes;			// The number of nodes in the BSP
	int m_numOfLeafs;			// The number of leafs
	int m_numOfLeafFaces;		// The number of faces
	int m_numOfPlanes;			// The number of planes in the BSP
	int m_numOfBrushes;			// The number of brushes in our world
	int m_numOfBrushSides;		// The number of brush sides in our world
	int m_numOfLeafBrushes;		// The number of leaf brushes
	int m_numOfModels;

	int			  *m_pIndices;		// The object's indices for rendering
	tBSPVertex    *m_pVerts;		// The object's vertices
	tBSPFace      *m_pFaces;		// The faces information of the object
	tBSPNode      *m_pNodes;		// The nodes in the BSP
	tBSPLeaf      *m_pLeafs;		// The leafs in the BSP
	tBSPPlane     *m_pPlanes;		// The planes in the BSP
	int           *m_pLeafFaces;	// The leaf faces in the BSP
	tBSPVisData   m_clusters;		// The cluster info for frustum culling and portals (PVS)
	tBSPTexture   *m_pTextures;		// This stores our texture info for each brush
	tBSPBrush	  *m_pBrushes;		// This is our brushes
	tBSPBrushSide *m_pBrushSides;	// This holds the brush sides
	int			  *m_pLeafBrushes;  // The indices into the brush array
	tModel		  *m_pModels;
								
	GLuint m_textures[MAX_TEXTURES];	// The texture array for the world
	GLuint m_lightmaps[MAX_TEXTURES];	// The lightmap texture array
								
	CBitset m_FacesDrawn;			// The bitset for the faces that have/haven't been drawn

	char * entities;
	EntityManager m_entityManager;
};


#pragma once

#include <stdio.h>
#include <string.h>
#include "Q3Types.h"

#define MAX_TEXTURES 10
#define FACE_POLYGON	1

#include "../Renderer.h"

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
    int textureID;				// The index into the texture array 
    int effect;					// The index for the effects (or -1 = n/a) 
    int type;					// 1=polygon, 2=patch, 3=mesh, 4=billboard 
    int startVertIndex;			// The starting index into this face's first vertex 
    int numOfVerts;				// The number of vertices for this face 
    int startIndex;				// The starting index into the indices array for this face
    int numOfIndices;			// The number of indices for this face
    int lightmapID;				// The texture index for the lightmap 
    int lMapCorner[2];			// The face's lightmap corner in the image 
    int lMapSize[2];			// The size of the lightmap section 
    Q3Vector3 lMapPos;			// The 3D origin of lightmap. 
    Q3Vector3 lMapVecs[2];		// The 3D space for s and t unit vectors. 
    Q3Vector3 vNormal;			// The face normal. 
    int size[2];				// The bezier patch dimensions. 
};

struct tBSPTexture
{
    char strName[64];
    int flags;
    int contents;
};

// This stores a node in the BSP tree
struct tBSPNode
{
    int plane;					// The index into the planes array 
    int front;					// The child index for the front node 
    int back;					// The child index for the back node 
    tVector3i min;				// The bounding box min position. 
    tVector3i max;				// The bounding box max position. 
}; 

// This stores a leaf (end node) in the BSP tree
struct tBSPLeaf
{
    int cluster;				// The visibility cluster 
    int area;					// The area portal 
    tVector3i min;				// The bounding box min position 
    tVector3i max;				// The bounding box max position 
    int leafface;				// The first index into the face array 
    int numOfLeafFaces;			// The number of faces for this leaf 
    int leafBrush;				// The first index for into the brushes 
    int numOfLeafBrushes;		// The number of brushes for this leaf 
}; 

// This stores a splitter plane in the BSP tree
struct tBSPPlane
{
    Q3Vector3 vNormal;			// Plane normal. 
    float d;					// The plane distance from origin 
};

struct tBSPVisData
{
	int numOfClusters;
	int bytesPerCluster;
	char *pBitsets;				
};

enum eLumps
{
    kEntities = 0,				// Stores player/object positions, etc...
    kTextures,					// Stores texture information
    kPlanes,				    // Stores the splitting planes
    kNodes,						// Stores the BSP nodes
    kLeafs,						// Stores the leafs of the nodes
    kLeafFaces,					// Stores the leaf's indices into the faces
    kLeafBrushes,				// Stores the leaf's indices into the brushes
    kModels,					// Stores the info of world models
    kBrushes,					// Stores the brushes info (for collision)
    kBrushSides,				// Stores the brush surfaces info
    kVertices,					// Stores the level vertices
    kIndices,					// Stores the level indices
    kShaders,					// Stores the shader files (blending, anims..)
    kFaces,						// Stores the faces for the level
    kLightmaps,					// Stores the lightmaps for the level
    kLightVolumes,				// Stores extra world lighting information
    kVisData,					// Stores PVS and cluster info (visibility)
    kMaxLumps					// A constant to store the number of lumps
};

class Q3Bitset 
{

public:

    Q3Bitset() : m_bits(0), m_size(0) {}

	~Q3Bitset() 
	{
		if(m_bits) 
		{
			delete m_bits;
			m_bits = 0;
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

	bool LoadBSP(const char *strFileName, Renderer *renderer);
	void RenderLevel(Vec3 *position, Renderer *renderer, Mat4x4 *viewProjectionMatrix);

	void Destroy();

private:

	Vertex *vertices;

	int IsClusterVisible(int current, int test);
	int FindLeaf(const Vec3 *position);

	void FindTextureExtension(char *strFileName);

	void RenderFace(int faceIndex, Renderer *renderer);

	int m_numOfVerts;			// The number of verts in the model
	int m_numOfFaces;			// The number of faces in the model
	int m_numOfIndices;			// The number of indices for the model
	int m_numOfTextures;		// The number of texture maps
	int m_numOfLightmaps;		// The number of light maps

	int m_numOfNodes;			// The number of nodes in the level
	int m_numOfLeafs;			// The leaf count in the level
	int m_numOfLeafFaces;		// The number of leaf faces in the level
	int m_numOfPlanes;			// The number of planes in the level

	int			*m_pIndices;	// The object's indices for rendering
	tBSPVertex  *m_pVerts;		// The object's vertices
	tBSPFace    *m_pFaces;		// The faces information of the object



	tBSPNode    *m_pNodes;		// The nodes in the bsp tree
	tBSPLeaf    *m_pLeafs;		// The leafs in the bsp tree
	tBSPPlane   *m_pPlanes;		// The planes stored in the bsp tree
	int         *m_pLeafFaces;	// The leaf's faces in the bsp tree
	tBSPVisData  m_clusters;	// The clusters in the bsp tree for space partitioning

	unsigned int m_textures[MAX_TEXTURES];	
	unsigned int m_lightmaps[MAX_TEXTURES];
								
	Q3Bitset m_FacesDrawn;		// The bitset for the faces that have/haven't been drawn
};

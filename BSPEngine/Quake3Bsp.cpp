#include "Main.h"
#include "Camera.h"
#include "Quake3Bsp.h"
#include "Frustum.h"

int NO_DRAW_TEXTURE_ID = -666;

// Q3BSP functionality makes use of code and logic from the following sources:
// <cite> Morgan McGuire, Rendering Quake 3 Maps  http://graphics.cs.brown.edu/games/quake/quake3.html </cite>
// and
// <cite> Ben "Digiben" Humphrey, Unofficial  Quake 3 BSP Format http://www.misofruit.co.kr/seojewoo/programming/opengl/Quake3Format.htm </cite>

Q3BSP::Q3BSP()
{
	m_numOfVerts		= 0;	
	m_numOfFaces		= 0;	
	m_numOfIndices		= 0;	
	m_numOfTextures		= 0;
	m_numOfLightmaps	= 0;
	m_numOfNodes		= 0;
	m_numOfLeafs		= 0;
	m_numOfLeafFaces	= 0;
	m_numOfPlanes		= 0;
	m_numOfBrushes		= 0;
	m_numOfBrushSides	= 0;
	m_numOfLeafBrushes	= 0;
	m_numOfModels		= 0;
	
	m_pVerts		 = NULL;	
	m_pFaces		 = NULL;	
	m_pIndices		 = NULL;	
	m_pNodes		 = NULL;
	m_pLeafs		 = NULL;
	m_pPlanes		 = NULL;
	m_pLeafFaces	 = NULL;

	memset(&m_clusters, 0, sizeof(tBSPVisData));

	m_pBrushes       = NULL;
	m_pBrushSides	 = NULL;
	m_pTextures      = NULL;
	m_pLeafBrushes	 = NULL;

	physicsManager.Initialize();
	physicsManager.CreateGroundPlane();
}

void Q3BSP::ChangeGamma(unsigned char *pImage, int size, float factor)
{
	for(int i = 0; i < size / 3; i++, pImage += 3) 
	{
		float scale = 1.0f, temp = 0.0f;
		float r = 0, g = 0, b = 0;

		r = (float)pImage[0];
		g = (float)pImage[1];
		b = (float)pImage[2];

		r = r * factor / 255.0f;
		g = g * factor / 255.0f;
		b = b * factor / 255.0f;
		
		if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;
		if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;
		if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;

		scale*=255.0f;		
		r*=scale;	g*=scale;	b*=scale;

		pImage[0] = (unsigned char)r;
		pImage[1] = (unsigned char)g;
		pImage[2] = (unsigned char)b;
	}
}

void Q3BSP::CreateLightmapTexture(GLuint &texture, unsigned char *pImageBits, int width, int height)
{
	glGenTextures(1, &texture);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texture);

	//ChangeGamma(pImageBits, width*height*3, g_Gamma);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pImageBits);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

float Dot(Vector3 vVector1, Vector3 vVector2) 
{
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}

bool Q3BSP::LoadBSP(const char *strFileName)
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
	m_pTextures = new tBSPTexture [m_numOfTextures];

	m_numOfLightmaps = lumps[kLightmaps].length / sizeof(tBSPLightmap);
	tBSPLightmap *pLightmaps = new tBSPLightmap [m_numOfLightmaps ];

	fseek(fp, lumps[kVertices].offset, SEEK_SET);

	for(i = 0; i < m_numOfVerts; i++)
	{
		fread(&m_pVerts[i], 1, sizeof(tBSPVertex), fp);
		
		// Swap the y and z values, and negate the new z so Y is up.
		float temp = m_pVerts[i].vPosition.y;
		m_pVerts[i].vPosition.y = m_pVerts[i].vPosition.z;
		m_pVerts[i].vPosition.z = -temp;

		// Does this need to be done for the normals too?
		temp = m_pVerts[i].vNormal.y;
		m_pVerts[i].vNormal.y = m_pVerts[i].vNormal.z;
		m_pVerts[i].vNormal.z = -temp;
	}	

	fseek(fp, lumps[kIndices].offset, SEEK_SET);

	fread(m_pIndices, m_numOfIndices, sizeof(int), fp);

	fseek(fp, lumps[kFaces].offset, SEEK_SET);

	fread(m_pFaces, m_numOfFaces, sizeof(tBSPFace), fp);

	fseek(fp, lumps[kTextures].offset, SEEK_SET);
	
	fread(m_pTextures, m_numOfTextures, sizeof(tBSPTexture), fp);

	for(i = 0; i < m_numOfTextures; i++)
	{		
		CreateTexture(m_textures[i], m_pTextures[i].strName);
	}

	//char normalname[64] = "textures/firstmap/normals";
	//CreateTexture(m_textures[m_numOfTextures+1], normalname);

	//CreateTexture(m_lightmaps[9], "textures/firstmap/grey_normal");

	fseek(fp, lumps[kLightmaps].offset, SEEK_SET);

	for(i = 0; i < m_numOfLightmaps ; i++)
	{
		fread(&pLightmaps[i], 1, sizeof(tBSPLightmap), fp);

		CreateLightmapTexture(m_lightmaps[i], (unsigned char *)pLightmaps[i].imageBits, 128, 128);
	}

	delete [] pLightmaps;

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
		// Swap the min y and z values, then negate the new Z
		int temp = m_pLeafs[i].min.y;
		m_pLeafs[i].min.y = m_pLeafs[i].min.z;
		m_pLeafs[i].min.z = -temp;

		// Swap the max y and z values, then negate the new Z
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
		m_clusters.pBitsets = new unsigned char [size];

		fread(m_clusters.pBitsets, 1, sizeof(unsigned char) * size, fp);
	}
	else
		m_clusters.pBitsets = NULL;

	m_numOfBrushes = lumps[kBrushes].length / sizeof(int);
	m_pBrushes = new tBSPBrush [m_numOfBrushes];
	
	fseek(fp, lumps[kBrushes].offset, SEEK_SET);
	fread(m_pBrushes, m_numOfBrushes, sizeof(tBSPBrush), fp);

	m_numOfBrushSides = lumps[kBrushSides].length / sizeof(int);
	m_pBrushSides = new tBSPBrushSide [m_numOfBrushSides];

	fseek(fp, lumps[kBrushSides].offset, SEEK_SET);
	fread(m_pBrushSides, m_numOfBrushSides, sizeof(tBSPBrushSide), fp);

	m_numOfLeafBrushes = lumps[kLeafBrushes].length / sizeof(int);
	m_pLeafBrushes = new int [m_numOfLeafBrushes];

	fseek(fp, lumps[kLeafBrushes].offset, SEEK_SET);
	fread(m_pLeafBrushes, m_numOfLeafBrushes, sizeof(int), fp);

	// Just dump the entities to a log right now so that they can be inspected by a human
	{
		entities = new char [lumps[kEntities].length];
		fseek(fp, lumps[kEntities].offset, SEEK_SET);
		fread(entities, lumps[kEntities].length, 1, fp);

		FILE * hFile = fopen( "entitieslog.log", "w"); // w clears out the file on write
		fwrite (entities , 1, lumps[kEntities].length, hFile );
		fclose(hFile);
	}

	LoadModels(lumps, fp);

	fclose(fp);

	m_FacesDrawn.Resize(m_numOfFaces);

	{ // Here we are going to try and push into the physics engine a mesh interpretation of the level geometry

		tModel * worldspawn = &m_pModels[0];

		for (int f = worldspawn->firstface; f < worldspawn->firstface + worldspawn->numfaces; f++)
		{
			tBSPFace *pFace = &m_pFaces[f];

			NxVec3 verts[512]; // Create a new buffer to hold enough NxVec3's to represent all our vertices
			NxU16 tris[512]; // we could use real sizes here but this makes it a bit easier to see in the debugger whats happening

			int vertCounter = 0;

			for (int i = pFace->startVertIndex; i < pFace->startVertIndex + pFace->numOfVerts; i++)
			{
				verts[vertCounter] = NxVec3(m_pVerts[i].vPosition.x, m_pVerts[i].vPosition.y, m_pVerts[i].vPosition.z);
				vertCounter++;
			}

			int trisCounter = 0;

			for (int i = pFace->startIndex; i < pFace->startIndex + pFace->numOfIndices; i++)
			{
				tris[trisCounter] = m_pIndices[i];
				trisCounter++;
			}

			for (int i = 0; i < trisCounter; i+=3) // Fix triangle winding to change direction of normals
			{
				NxU16 temp = tris[i];
				tris[i] = tris[i+1];
				tris[i+1] = temp;
			}

			NxActor *actor = physicsManager.CreateTriangleMesh(pFace->numOfVerts, pFace->numOfIndices, verts, tris);		
		}
	}

	return true;
}

void Q3BSP::LoadModels(tBSPLump *lumps, FILE *fp)
{
	int i, j, m;

	m_numOfModels = lumps[kModels].length / sizeof(int);
	m_pModels = new tModel [m_numOfModels];

	fseek(fp, lumps[kModels].offset, SEEK_SET);
	fread(m_pModels, m_numOfModels, sizeof(tModel), fp);

	m_entityManager.ProcessEntities(entities);

	// Associate entities with model
	for (i = 0; i < m_entityManager.numEntities; i++)
	{
		m = m_entityManager.entities[i].model;
		if (m > 0 && m < m_numOfModels)
		{
			m_entityManager.entities[i].SetModel(&m_pModels[m], 0);

			// After we set the model we have some idea of its bounding box so we can put that into its actor description
			
			tModel *m = m_entityManager.entities[i].models[0];

			float width =  fabs(m->maxs[0] - m->mins[0]) / 2.0f;
			float height = fabs(m->maxs[1] - m->mins[1]) / 2.0f;
			float depth =  fabs(m->maxs[2] - m->mins[2]) / 2.0f;

			NxVec3 dimensions(width, depth, height);
			if (m_entityManager.entities[i].enttype == EntityManager::FUNC_STATIC)
			{
				m_entityManager.entities[i].actor = physicsManager.CreateBox(&dimensions, true);
			}
			else
			{
				m_entityManager.entities[i].actor = physicsManager.CreateBox(&dimensions, false);
			}
			m_entityManager.entities[i].actor->userData = &m_entityManager.entities[i];
			m_entityManager.entities[i].actor->setGlobalPosition(NxVec3(m_entityManager.entities[i].origin.x, m_entityManager.entities[i].origin.y, m_entityManager.entities[i].origin.z));
			m_entityManager.entities[i].Update(0.001f);
		}

		// Associate children
		for (j = 0; j < m_entityManager.entities[i].numchildren; ++j)
		{
			m = m_entityManager.entities[i].children[j]->model;
			if (m > 0 && m < m_numOfModels)
			{
				m_entityManager.entities[i].children[j]->SetModel(&m_pModels[m], 0);
			}
		}
	}
}

int Q3BSP::FindLeaf(const Vector3 &vPos)
{
	int i = 0;
	float distance = 0.0f;
	
	while(i >= 0)
	{
		const tBSPNode&  node = m_pNodes[i];
		const tBSPPlane& plane = m_pPlanes[node.plane];

        distance =	plane.vNormal.x * vPos.x + plane.vNormal.y * vPos.y + plane.vNormal.z * vPos.z - plane.d;

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

	unsigned char visSet = m_clusters.pBitsets[(current*m_clusters.bytesPerCluster) + (test / 8)];
	
	int result = visSet & (1 << ((test) & 7));

	return ( result );
}

void Q3BSP::RenderFace(int faceIndex, DefaultShader *shader)
{
	tBSPFace *pFace = &m_pFaces[faceIndex];

	if ( m_textures[pFace->textureID] == NO_DRAW_TEXTURE_ID )
	{
		return;
	}

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(tBSPVertex), &(m_pVerts[pFace->startVertIndex].vPosition));
	glNormalPointer(GL_FLOAT, sizeof(tBSPVertex), &(m_pVerts[pFace->startVertIndex].vNormal));
	
	//if(g_bTextures)
	{
		glActiveTexture(GL_TEXTURE0);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, sizeof(tBSPVertex), &(m_pVerts[pFace->startVertIndex].vTextureCoord));

		glClientActiveTexture(GL_TEXTURE0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,  m_textures[pFace->textureID]);
		glUniform1i(shader->GetDiffuseLocation(), 0);
	}

	//if(g_bLightmaps)
	{
		glActiveTexture(GL_TEXTURE1);

		glClientActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, sizeof(tBSPVertex), &(m_pVerts[pFace->startVertIndex].vLightmapCoord));

		glClientActiveTexture(GL_TEXTURE1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,  m_lightmaps[pFace->lightmapID]);
		glUniform1i(shader->GetLightmapLocation(), 1);
	}

	glDrawElements(GL_TRIANGLES, pFace->numOfIndices, GL_UNSIGNED_INT, &(m_pIndices[pFace->startIndex]) );

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}

void Q3BSP::RenderModel(tModel *model, DefaultShader *shader)
{
	int faceCount = model->numfaces;

	while(faceCount--)
	{
		int faceIndex = model->firstface + faceCount;

		RenderFace(faceIndex, shader);
	}
}

void Q3BSP::RenderLevel(const Vector3 &vPos, DefaultShader *shader, Frustum *frustum)
{
	m_FacesDrawn.ClearAll();

	int leafIndex = FindLeaf(vPos);

	int cluster = m_pLeafs[leafIndex].cluster;

	int i = m_numOfLeafs;

	while(i--)
	{
		tBSPLeaf *pLeaf = &(m_pLeafs[i]);

		if(!IsClusterVisible(cluster, pLeaf->cluster)) 
		{
			continue;
		}

		if(!frustum->BoxInFrustum((float)pLeaf->min.x, (float)pLeaf->min.y, (float)pLeaf->min.z, (float)pLeaf->max.x, (float)pLeaf->max.y, (float)pLeaf->max.z))
		{
			continue;
		}
		
		int faceCount = pLeaf->numOfLeafFaces;

		while(faceCount--)
		{
			int faceIndex = m_pLeafFaces[pLeaf->leafface + faceCount];

			if(!m_FacesDrawn.On(faceIndex)) 
			{
				m_FacesDrawn.Set(faceIndex);
				glUseProgram(shader->program);
				RenderFace(faceIndex, shader);
			}
		}			
	}

	glDisable(GL_CULL_FACE); // Turn this off so that our models will reflect nicely

	for (int i = 0; i < m_entityManager.numEntities; i++)
	{
		tModel * model = m_entityManager.entities[i].models[0]; // This zero indicates which LOD of the model we want to draw (I am assuming 0 is the highest)
		
		if (model) // Some entities dont have models (player spawn etc)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

				glMultMatrixf(m_entityManager.entities[i].transformation);

				RenderModel(model, shader);

			glPopMatrix();
		}
	}

	glEnable(GL_CULL_FACE);
}

float elapsedtime = 0;
void Q3BSP::Update(float milliseconds)
{
	elapsedtime += milliseconds * 0.001f;;

	for (int i = 0; i < m_entityManager.numEntities; i++)
	{
		m_entityManager.entities[i].Update(elapsedtime);
	}

	physicsManager.Update(milliseconds);
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

	if(m_pBrushes)	
	{
		delete [] m_pBrushes;		m_pBrushes = NULL;
	}
	
	if(m_pBrushSides)	
	{
		delete [] m_pBrushSides;	m_pBrushSides = NULL;
	}

	if(m_pLeafBrushes)	
	{
		delete [] m_pLeafBrushes;	m_pLeafBrushes = NULL;
	}

	if(m_pTextures)	
	{
		delete [] m_pTextures;		m_pTextures = NULL;
	}	

	glDeleteTextures(m_numOfTextures, m_textures);

	glDeleteTextures(m_numOfLightmaps, m_lightmaps);
}

Q3BSP::~Q3BSP()
{
	Destroy();
}

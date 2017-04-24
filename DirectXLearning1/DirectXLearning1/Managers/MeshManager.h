#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

struct MeshMaterialNum
{
	LPD3DXMESH mesh;
	LPD3DXBUFFER matBuffer;
	DWORD num;
};

class MeshManager
{
public:
	MeshManager(LPDIRECT3DDEVICE9 direct3DDevice);
	~MeshManager();

	LPD3DXMESH GetMesh(const char *filename, LPD3DXBUFFER &bufShipMaterial, DWORD &numMaterials);

private:
	map <string, MeshMaterialNum> m_Meshes;

	LPDIRECT3DDEVICE9 m_Direct3DDevice;
};

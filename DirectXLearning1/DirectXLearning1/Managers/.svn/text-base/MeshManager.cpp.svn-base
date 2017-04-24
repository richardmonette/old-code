#include "MeshManager.h"

MeshManager::MeshManager(LPDIRECT3DDEVICE9 direct3DDevice)
{
	m_Direct3DDevice = direct3DDevice;
}

MeshManager::~MeshManager()
{
	map<string, MeshMaterialNum>::iterator it;

	for ( it = m_Meshes.begin(); it != m_Meshes.end(); it++ )
	{
		(*it).second.mesh->Release();
		(*it).second.matBuffer->Release();
	}
}

LPD3DXMESH MeshManager::GetMesh(const char *filename, LPD3DXBUFFER &material, DWORD &numMaterials)
{
	if (m_Meshes.find(filename) == m_Meshes.end())
	{
		LPD3DXMESH mesh;

		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, m_Direct3DDevice, NULL, &material, NULL, &numMaterials, &mesh);

		/*if( !( mesh->GetFVF() & D3DFVF_NORMAL ) )
		{
			LPD3DXMESH pTempMesh;
			mesh->CloneMeshFVF( mesh->GetOptions(), mesh->GetFVF() | D3DFVF_NORMAL, m_Direct3DDevice, &pTempMesh );
			D3DXComputeNormals( pTempMesh, NULL );

			mesh->Release();
			mesh = pTempMesh;
		}*/

		MeshMaterialNum meshMatNum;
		meshMatNum.mesh = mesh;
		meshMatNum.matBuffer = material;
		meshMatNum.num = numMaterials;
		m_Meshes[filename] = meshMatNum;
	}

	material = m_Meshes[filename].matBuffer;
	numMaterials = m_Meshes[filename].num;

	return m_Meshes[filename].mesh;
}

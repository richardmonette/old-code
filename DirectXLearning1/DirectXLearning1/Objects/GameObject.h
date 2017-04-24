#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "../Global.h"
#include "../AABB.h"
#include "Interactive3D.h"

#include "../Physics/PhysicsManager.h"

class GameObject : Interactive3D
{
public:
	GameObject(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 direct3DDevice, const char *name);
	~GameObject();

	void Draw();
	virtual void Update();

	NxVec3 GetPosition();
	void SetPosition(NxVec3 *position);

	unsigned int GetNumMaterials();

	void SetSelected(bool selected);
	bool GetSelected();

	void SetSelectable(bool selected);
	bool GetSelectable();

	LPDIRECT3DTEXTURE9 GetTexture(unsigned int index);
	D3DMATERIAL9 * GetMaterial(unsigned int index);

	void AddMaterial(D3DMATERIAL9 * material);
	void AddTexture(LPDIRECT3DTEXTURE9 texture);

	const char * GetMeshFilename();
	const char * GetName();

	LPD3DXMESH GetMesh();
	void SetMesh(const char *filename, LPD3DXMESH mesh, bool isStatic);

	virtual void HandleMouseDown();
	virtual void HandleMouseUp();

	NxActor * m_Actor;

private:

	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	void CreateAABB();

	//Note this AABB is going to be in local space
	AABB m_ModelBoundingBox;

	string m_Name;
	string m_MeshFilename;
	
	LPD3DXMESH m_Mesh;
	
	//TODO: Move these out of the GameObject and into the Texture and Material managers
	vector<D3DMATERIAL9*> m_Materials;
	vector<LPDIRECT3DTEXTURE9> m_Textures;

	bool m_Selectable;
	bool m_Selected;

	D3DXMATRIX m_PositionMatrix;
	D3DXVECTOR3 m_Position;
};

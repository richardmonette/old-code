#pragma once

#include "Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class AABB
{
public:
	AABB();
	~AABB();

	void Draw(LPDIRECT3DDEVICE9 direct3DDevice);

	void SetMinMax(D3DXVECTOR3 &minBounds, D3DXVECTOR3 &maxBounds, LPDIRECT3DDEVICE9 direct3DDevice);

	D3DXVECTOR3 * GetMinBounds();
	D3DXVECTOR3 * GetMaxBounds();

private:

	D3DXVECTOR3 m_MinBounds;
	D3DXVECTOR3 m_MaxBounds;

	D3DXVECTOR3 m_ObjectBounds[8];

	LPD3DXMESH m_BoxMesh;
};

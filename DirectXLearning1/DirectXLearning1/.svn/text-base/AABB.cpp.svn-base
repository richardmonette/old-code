#include "AABB.h"

AABB::AABB()
{
}

AABB::~AABB()
{
	m_BoxMesh->Release();
}

void AABB::Draw(LPDIRECT3DDEVICE9 direct3DDevice)
{
	direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	direct3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_BoxMesh->DrawSubset(0);

	//TOFIX: This fill mode is going to be overriding the fill mode setting in the engine
	direct3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID );
	direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	direct3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void AABB::SetMinMax(D3DXVECTOR3 &minBounds, D3DXVECTOR3 &maxBounds, LPDIRECT3DDEVICE9 direct3DDevice)
{
	m_MinBounds = minBounds;
	m_MaxBounds = maxBounds;

	m_ObjectBounds[0] = D3DXVECTOR3( minBounds.x, minBounds.y, minBounds.z ); // xyz
	m_ObjectBounds[1] = D3DXVECTOR3( maxBounds.x, minBounds.y, minBounds.z ); // Xyz
	m_ObjectBounds[2] = D3DXVECTOR3( minBounds.x, maxBounds.y, minBounds.z ); // xYz
	m_ObjectBounds[3] = D3DXVECTOR3( maxBounds.x, maxBounds.y, minBounds.z ); // XYz
	m_ObjectBounds[4] = D3DXVECTOR3( minBounds.x, minBounds.y, maxBounds.z ); // xyZ
	m_ObjectBounds[5] = D3DXVECTOR3( maxBounds.x, minBounds.y, maxBounds.z ); // XyZ
	m_ObjectBounds[6] = D3DXVECTOR3( minBounds.x, maxBounds.y, maxBounds.z ); // xYZ
	m_ObjectBounds[7] = D3DXVECTOR3( maxBounds.x, maxBounds.y, maxBounds.z ); // XYZ

	D3DXCreateBox(direct3DDevice, abs(minBounds.x-maxBounds.x), abs(minBounds.y-maxBounds.y), abs(minBounds.z-maxBounds.z), &m_BoxMesh, NULL);
}

D3DXVECTOR3 * AABB::GetMinBounds()
{
	return &m_MinBounds;
}

D3DXVECTOR3 * AABB::GetMaxBounds()
{
	return &m_MaxBounds;
}

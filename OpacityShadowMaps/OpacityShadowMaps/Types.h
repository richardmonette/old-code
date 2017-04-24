
#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

struct ConstantBufferType
{
	D3DXMATRIX gViewProj;
	D3DXMATRIX gLightViewProj;

	D3DXVECTOR3 gEyePosW;
	float temp;

	D3DXVECTOR3 gLightPosW;
	float temp1;
};

struct ParticleVertex
{
	D3DXVECTOR4 centerW;
	D3DXVECTOR4 sizeW;
	D3DXVECTOR4 color;
	D3DXVECTOR4 normal;
	float distance;
};

struct GeometryVertex
{
	D3DXVECTOR4 position;
	D3DXVECTOR4 uv;
	D3DXVECTOR4 normal;
};
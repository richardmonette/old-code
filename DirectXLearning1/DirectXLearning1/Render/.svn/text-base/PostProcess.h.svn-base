
#include <d3dx9.h>

#ifndef INC_POSTPROCESS_H
#define INC_POSTPROCESS_H


class PostProcess
{

public:

	PostProcess();


HRESULT CreateResources( IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pDisplayDesc );
HRESULT DestroyResources();
HRESULT PerformPostProcessing( IDirect3DDevice9* pDevice, HDRScene *hdrScene );
HRESULT GetTexture( IDirect3DTexture9** pTexture );
float GetBrightPassThreshold();
void SetBrightPassThreshold( const float& threshold );

float GetGaussianMultiplier();
void SetGaussianMultiplier( const float& multiplier );

float GetGaussianMean();
void SetGaussianMean( const float& mean );

float GetGaussianStandardDeviation();
void SetGaussianStandardDeviation( const float& sd );
HRESULT DisplaySteps( IDirect3DDevice9* pDevice, ID3DXFont* pFont, ID3DXSprite* pTextSprite, IDirect3DTexture9* pArrowTex );
DWORD CalculateResourceUsage();

HRESULT RenderToTexture( IDirect3DDevice9* pDev );

float ComputeGaussianValue( float x, float mean, float std_deviation );

private:

	LPDIRECT3DTEXTURE9 g_pBrightPassTex;
	LPDIRECT3DTEXTURE9 g_pDownSampledTex;
	LPDIRECT3DTEXTURE9 g_pBloomHorizontal;
	LPDIRECT3DTEXTURE9 g_pBloomVertical;

	D3DFORMAT g_fmtHDR;

	LPDIRECT3DPIXELSHADER9 g_pBrightPassPS;
	LPD3DXCONSTANTTABLE g_pBrightPassConstants;

	LPDIRECT3DPIXELSHADER9 g_pDownSamplePS;
	LPD3DXCONSTANTTABLE g_pDownSampleConstants;

	LPDIRECT3DPIXELSHADER9 g_pHBloomPS;
	LPD3DXCONSTANTTABLE g_pHBloomConstants;

	LPDIRECT3DPIXELSHADER9 g_pVBloomPS;
	LPD3DXCONSTANTTABLE g_pVBloomConstants;

	float g_BrightThreshold;
	float g_GaussMultiplier;
	float g_GaussMean;
	float g_GaussStdDev;

};

#endif // INC_POSTPROCESS_H


#ifndef INC_HDRSCENE_H
#define INC_HDRSCENE_H

#include <d3dx9.h>

struct Light 
{
	D3DXVECTOR3 position;
	float radius;
	float intensity;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 color;
	float outerCone;
	float innerCone;
};

class HDRScene
{

public:

	HDRScene();

	HRESULT CreateResources( IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pDisplayDesc );
	HRESULT DestroyResources();
	HRESULT RenderScene( IDirect3DDevice9* pDevice );
	HRESULT GetOutputTexture( IDirect3DTexture9** pTexture );
	HRESULT FullScreenQuad( IDirect3DDevice9* pDev );

	void LoadLightSphereMesh();
	
	void DrawLights(IDirect3DDevice9* pDevice);
	void DrawLight(IDirect3DDevice9* pDevice, Light *light);

	// TODO: Hide this in a getter
	ID3DXEffect * m_GBufferEffect;

private:

	vector<Light*> m_Lights;

	LPD3DXCONSTANTTABLE m_FinalCombineVSConsts;
	LPDIRECT3DPIXELSHADER9 m_FinalCombinePS;
	LPD3DXCONSTANTTABLE m_FinalCombinePSConsts;
	LPDIRECT3DVERTEXSHADER9 m_FinalCombineVS;

	ID3DXEffect * m_LightEffect;

	LPDIRECT3DTEXTURE9 m_RenderedHDRTexture;

	LPDIRECT3DTEXTURE9 m_GBufferColorTexture;    // Color and Specular Intensity
	LPDIRECT3DTEXTURE9 m_GBufferNormalTexture; // Normal and Specular power
	LPDIRECT3DTEXTURE9 m_GBufferDepthTexture; // Depth

	LPDIRECT3DTEXTURE9 m_LightMapTexture;

	D3DXVECTOR2 m_HalfPixel;

	LPD3DXMESH m_LightSphereMesh;

};


#endif // INC_HDRSCENE_H

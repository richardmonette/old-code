
#include <d3d9.h>
#include <d3dx9.h>

#include "../Global.h"
#include "HDRScene.h"
#include "PostProcess.h"
#include "Luminance.h"

class HDRPipeline
{
public:

	static HDRPipeline* GetInstance();
	~HDRPipeline();

	void Initialize(HWND hWnd, HINSTANCE hInstance);
	void Deinitialize();

	LPDIRECT3DDEVICE9 GetDirect3DDevice();
	LPDIRECT3D9 GetDirect3DObject();

	LPD3DXEFFECT GetVertexShaderConstantTable();

	HDRScene * GetHDRScene();

	struct TLVertex
	{
		D3DXVECTOR4 p;
		D3DXVECTOR2 t;
	};

	static const DWORD          FVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

	LPDIRECT3DPIXELSHADER9      g_pFinalPassPS;
	LPD3DXCONSTANTTABLE         g_pFinalPassPSConsts;
	float                       g_fExposure;

	D3DFORMAT                   g_FloatRenderTargetFmt;
	D3DFORMAT                   g_DepthFormat;
	LPDIRECT3DTEXTURE9          g_pFinalTexture;


	HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	void Render();
	void OnLostDevice( void* pUserContext );
	void Destroy();


protected:

	HDRPipeline();

private:

	HDRScene *m_HDRScene;
	PostProcess *m_PostProcess;
	Luminance *m_Luminance;

	void InitializeDirect3D(HWND hWnd);
	void CleanupDirect3D();

	LPDIRECT3D9 m_Direct3D;
	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	void DrawHDRTextureToScreen();

	static HDRPipeline* pInstance;
};
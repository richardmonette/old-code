
#include <d3dx9.h>

// Inclusion Guards
#ifndef INC_LUMINANCE_H
#define INC_LUMINANCE_H

// All functionality offered by this particular module
// is being wrapped up in a C++ namespace so as to make
// it clear what functions/etc.. belong together.
class Luminance
{

public:

	Luminance();

// This function creates the various textures and shaders used
// to compute the overall luminance of a given scene.
HRESULT CreateResources( IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pDisplayDesc );

// The functional opposite to the above function - it's job is
// to make sure that all resources are cleanly and safely tidied up.
HRESULT DestroyResources();

// This is the core function for this module - it will perform all
// of the necessary rendering and sampling to compute the 1x1
// luminance texture.
HRESULT MeasureLuminance( IDirect3DDevice9* pDevice, HDRScene *hdrScene );

// This function will display all stages of the luminance chain
// so that the end-user can see exactly what happened.
HRESULT DisplayLuminance( IDirect3DDevice9* pDevice, ID3DXFont* pFont, ID3DXSprite* pTextSprite,
                          IDirect3DTexture9* pArrowTex );

// The final, 1x1 texture, result of this module is needed to
// compute the final image that the user sees, as such other
// parts of this example need access to the luminance texture.
HRESULT GetLuminanceTexture( IDirect3DTexture9** pTex );

// A simple statistic that reveals how much texture memory is
// used by this particular module.
DWORD ComputeResourceUsage();

private:

	LPDIRECT3DPIXELSHADER9 g_pLumDispPS;

	LPDIRECT3DPIXELSHADER9 g_pLum1PS;
	LPD3DXCONSTANTTABLE g_pLum1PSConsts;

	LPDIRECT3DPIXELSHADER9 g_pLum3x3DSPS;
	LPD3DXCONSTANTTABLE g_pLum3x3DSPSConsts;

	static const DWORD g_dwLumTextures = 6;
	// Be careful when changing this value, higher than 6 might
	// require you to implement code that creates an additional
	// depth-stencil buffer due to the luminance textures dimensions
	// being greater than that of the default depth-stencil buffer.
	LPDIRECT3DTEXTURE9 g_pTexLuminance[ g_dwLumTextures ];
	D3DFORMAT g_fmtHDR;

};

#endif // INC_LUMINANCE_H


#include "HDRPipeline.h"
#include "HDRScene.h"
#include "Luminance.h"
#include "PostProcess.h"
#include "../Managers/InputManager.h"
#include "../Utility/StringUtility.h"

HDRPipeline* HDRPipeline::pInstance = NULL;

HDRPipeline* HDRPipeline::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new HDRPipeline;
	}

	return pInstance;
}

HDRPipeline::HDRPipeline()
{
}

void HDRPipeline::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	g_pFinalPassPS = NULL;     // The pixel shader that maps HDR->LDR
	g_pFinalPassPSConsts = NULL;     // The interface for setting param/const data for the above PS
	g_fExposure = 0.50f;    // The exposure bias fed into the FinalPass.psh shader (Render )

	g_FloatRenderTargetFmt = D3DFMT_UNKNOWN;   // 128 or 64 bit rendering...
	g_DepthFormat = D3DFMT_UNKNOWN;   // How many bits of depth buffer are we using?
	g_pFinalTexture = NULL;

	InitializeDirect3D(hWnd);

	// TODO: Don't cheat making this desc
	D3DSURFACE_DESC desc;
	desc.Width = SCREEN_WIDTH;
	desc.Height = SCREEN_HEIGHT;
	desc.Format = D3DFMT_X8R8G8B8;

	m_Luminance = new Luminance();
	m_PostProcess = new PostProcess();
	m_HDRScene = new HDRScene();

	OnResetDevice(m_Direct3DDevice, &desc, NULL);

}

LPDIRECT3DDEVICE9 HDRPipeline::GetDirect3DDevice()
{
	return m_Direct3DDevice;
}

LPDIRECT3D9 HDRPipeline::GetDirect3DObject()
{
	return m_Direct3D;
}

void HDRPipeline::InitializeDirect3D(HWND hWnd)
{
	m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

	for (UINT Adapter = 0; Adapter < m_Direct3D->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res;
		Res = m_Direct3D->GetAdapterIdentifier(Adapter,0,&Identifier);
		if (strstr(Identifier.Description, "PerfHUD") != 0)
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			break;
		}
	}

	m_Direct3D->CreateDevice(AdapterToUse, DeviceType, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_Direct3DDevice);

	m_Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_Direct3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

	m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 1);
	m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Direct3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

void HDRPipeline::CleanupDirect3D(void)
{
	m_Direct3DDevice->Release();
	m_Direct3D->Release();
}

HRESULT HDRPipeline::OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{

    HRESULT hr = S_OK;
    LPD3DXBUFFER pCode;

    pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

    pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    pd3dDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

    pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

    // Recreate the floating point resources
    pd3dDevice->CreateTexture( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
                                         1, D3DUSAGE_RENDERTARGET, pBackBufferSurfaceDesc->Format,
                                         D3DPOOL_DEFAULT, &g_pFinalTexture, NULL );


	string finalPassFilePath = StringUtility::GetContentPath();
	finalPassFilePath.append("shaders\\FinalPass.psh");
    D3DXCompileShaderFromFile( finalPassFilePath.c_str(),
                                         NULL, NULL,
                                         "main",
                                         "ps_2_0",
                                         0,
                                         &pCode,
                                         NULL,
                                         &g_pFinalPassPSConsts
                                         );

    pd3dDevice->CreatePixelShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ),
                                             &g_pFinalPassPS );

    pCode->Release();

    m_HDRScene->CreateResources( pd3dDevice, pBackBufferSurfaceDesc );
    m_Luminance->CreateResources( pd3dDevice, pBackBufferSurfaceDesc );
    m_PostProcess->CreateResources( pd3dDevice, pBackBufferSurfaceDesc );

    return S_OK;

}

HDRScene * HDRPipeline::GetHDRScene()
{
	return m_HDRScene;
}

LPD3DXEFFECT HDRPipeline::GetVertexShaderConstantTable()
{
	return m_HDRScene->m_GBufferEffect;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    //HDRScene::UpdateScene( DXUTGetD3D9Device(), static_cast< float >( fTime ), &g_Camera );
}


//--------------------------------------------------------------------------------------
//  Render()
//  ---------------
//  NOTES: This function makes particular use of the 'D3DPERF_BeginEvent()' and
//         'D3DPERF_EndEvent()' functions. See the documentation for more details,
//         but these are essentially used to provide better output from PIX. If you
//         perform a full call-stream capture on this sample, PIX will group together
//         related calls, making it much easier to interpret the results.
//--------------------------------------------------------------------------------------
void HDRPipeline::Render()
{

    //Cache some results for later use
    LPDIRECT3DSURFACE9 pLDRSurface = NULL;

    //Configure the render targets
    m_Direct3DDevice->GetRenderTarget( 0, &pLDRSurface );        //This is the output surface - a standard 32bit device

    // Clear the render target and the zbuffer 
    m_Direct3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0xFF, 0, 0, 0 ), 1.0f, 0 );

    // Render the scene
    if( SUCCEEDED( m_Direct3DDevice->BeginScene() ) )
    {

        // RENDER THE COMPLETE SCENE
        //--------------------------
        // The first part of each frame is to actually render the "core"
        // resources - those that would be required for an HDR-based pipeline.

        // HDRScene creates an unprocessed, raw, image to work with.
        D3DPERF_BeginEvent( D3DCOLOR_XRGB( 255, 0, 0 ), L"HDRScene Rendering" );
        m_HDRScene->RenderScene( m_Direct3DDevice );
        D3DPERF_EndEvent();

        // Luminance attempts to calculate what sort of tone/mapping should
        // be done as part of the post-processing step.
        D3DPERF_BeginEvent( D3DCOLOR_XRGB( 0, 0, 255 ), L"Luminance Rendering" );
        m_Luminance->MeasureLuminance( m_Direct3DDevice, m_HDRScene );
        D3DPERF_EndEvent();

        // The post-processing adds the blur to the bright (over-exposed) areas
        // of the image.
        D3DPERF_BeginEvent( D3DCOLOR_XRGB( 255, 0, 0 ), L"Post-Processing Rendering" );
        m_PostProcess->PerformPostProcessing( m_Direct3DDevice, m_HDRScene );
        D3DPERF_EndEvent();

        // When all the individual components have been created we have the final
        // composition. The output of this is the image that will be displayed
        // on screen.
        D3DPERF_BeginEvent( D3DCOLOR_XRGB( 255, 0, 255 ), L"Final Image Composition" );
        {
            LPDIRECT3DSURFACE9 pFinalSurf = NULL;
            g_pFinalTexture->GetSurfaceLevel( 0, &pFinalSurf );
            m_Direct3DDevice->SetRenderTarget( 0, pFinalSurf );

            LPDIRECT3DTEXTURE9 pHDRTex = NULL;
            m_HDRScene->GetOutputTexture( &pHDRTex );

            LPDIRECT3DTEXTURE9 pLumTex = NULL;
            m_Luminance->GetLuminanceTexture( &pLumTex );

            LPDIRECT3DTEXTURE9 pBloomTex = NULL;
            m_PostProcess->GetTexture( &pBloomTex );

            m_Direct3DDevice->SetTexture( 0, pHDRTex );
            m_Direct3DDevice->SetTexture( 1, pLumTex );
            m_Direct3DDevice->SetTexture( 2, pBloomTex );

            m_Direct3DDevice->SetPixelShader( g_pFinalPassPS );

            D3DSURFACE_DESC d;
            pBloomTex->GetLevelDesc( 0, &d );
            g_pFinalPassPSConsts->SetFloat( m_Direct3DDevice, "g_rcp_bloom_tex_w", 1.0f / static_cast< float >( d.Width ) );
            g_pFinalPassPSConsts->SetFloat( m_Direct3DDevice, "g_rcp_bloom_tex_h", 1.0f / static_cast< float >( d.Height ) );
            g_pFinalPassPSConsts->SetFloat( m_Direct3DDevice, "fExposure", g_fExposure );
            g_pFinalPassPSConsts->SetFloat( m_Direct3DDevice, "fGaussianScalar", m_PostProcess->GetGaussianMultiplier() );

			m_Direct3DDevice->SetRenderTarget( 0, pLDRSurface );
            
			DrawHDRTextureToScreen();

            m_Direct3DDevice->SetPixelShader( NULL );

            m_Direct3DDevice->SetTexture( 2, NULL );
            m_Direct3DDevice->SetTexture( 1, NULL );
            m_Direct3DDevice->SetTexture( 0, NULL );

            pBloomTex->Release();
            pLumTex->Release();
            pHDRTex->Release();
            pFinalSurf->Release();
        }
        D3DPERF_EndEvent();

			InputManager::GetInstance()->Render();

        // We've finished all of the rendering, so make sure that D3D
        // is aware of this...
        m_Direct3DDevice->EndScene();
    }

    //Remove any memory involved in the render target switching
    pLDRSurface->Release();

	m_Direct3DDevice->Present(NULL, NULL, NULL, NULL);

}

void HDRPipeline::Destroy()
{

    // Free up the HDR resources
    g_pFinalTexture->Release();

    // Free up the final screen pass resources
    g_pFinalPassPS->Release();
    g_pFinalPassPSConsts->Release();

    m_HDRScene->DestroyResources();
    m_Luminance->DestroyResources();
    m_PostProcess->DestroyResources();

}

//--------------------------------------------------------------------------------------
//  The last stage of the HDR pipeline requires us to take the image that was rendered
//  to an HDR-format texture and map it onto a LDR render target that can be displayed
//  on screen. This is done by rendering a screen-space quad and setting a pixel shader
//  up to map HDR->LDR.
//--------------------------------------------------------------------------------------
void HDRPipeline::DrawHDRTextureToScreen()
{
    // Find out what dimensions the screen is
    IDirect3DDevice9* pDev = GetDirect3DDevice();
    D3DSURFACE_DESC desc;
    LPDIRECT3DSURFACE9 pSurfRT;

    pDev->GetRenderTarget( 0, &pSurfRT );
    pSurfRT->GetDesc( &desc );
    pSurfRT->Release();

    // To correctly map from texels->pixels we offset the coordinates
    // by -0.5f:
    float fWidth = static_cast< float >( desc.Width ) - 0.5f;
    float fHeight = static_cast< float >( desc.Height ) - 0.5f;

    // Now we can actually assemble the screen-space geometry
    TLVertex v[4];

    v[0].p = D3DXVECTOR4( -0.5f, -0.5f, 0.0f, 1.0f );
    v[0].t = D3DXVECTOR2( 0.0f, 0.0f );

    v[1].p = D3DXVECTOR4( fWidth, -0.5f, 0.0f, 1.0f );
    v[1].t = D3DXVECTOR2( 1.0f, 0.0f );

    v[2].p = D3DXVECTOR4( -0.5f, fHeight, 0.0f, 1.0f );
    v[2].t = D3DXVECTOR2( 0.0f, 1.0f );

    v[3].p = D3DXVECTOR4( fWidth, fHeight, 0.0f, 1.0f );
    v[3].t = D3DXVECTOR2( 1.0f, 1.0f );

    // Configure the device and render..
    pDev->SetVertexShader( NULL );
    pDev->SetFVF( FVF_TLVERTEX );
    pDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( TLVertex ) );
}

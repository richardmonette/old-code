
#include "../Global.h"
#include "../Utility/StringUtility.h"
#include "HDRScene.h"
#include "Luminance.h"


//--------------------------------------------------------------------------------------
// Data Structure Definitions
//--------------------------------------------------------------------------------------
struct TLVertex
{
    D3DXVECTOR4 p;
    D3DXVECTOR2 t;
};

static const DWORD FVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

Luminance::Luminance()
{
	g_pLumDispPS = NULL;             // PShader used to display the debug panel

	g_pLum1PS = NULL;             // PShader that does a 2x2 downsample and convert to greyscale
	g_pLum1PSConsts = NULL;             // Interface to set the sampling points for the above PS

	g_pLum3x3DSPS = NULL;             // The PS that does each 3x3 downsample operation
	g_pLum3x3DSPSConsts = NULL;             // Interface for the above PS

	g_fmtHDR = D3DFMT_UNKNOWN;   // Should be either G32R32F or G16R16F depending on the hardware
}

//--------------------------------------------------------------------------------------
// Private Function Prototypes
//--------------------------------------------------------------------------------------
HRESULT RenderToTexture( IDirect3DDevice9* pDev );



//--------------------------------------------------------------------------------------
//  CreateResources( )
//
//      DESC:
//          This function creates the necessary texture chain for downsampling the
//          initial HDR texture to a 1x1 luminance texture.
//
//      PARAMS:
//          pDevice      : The current device that resources should be created with/from
//          pDisplayDesc : Describes the back-buffer currently in use, can be useful when
//                         creating GUI based resources.
//
//      NOTES:
//          n/a
//--------------------------------------------------------------------------------------
HRESULT Luminance::CreateResources( IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pDisplayDesc )
{

    //[ 0 ] DECLARATIONS
    //------------------
    HRESULT hr = S_OK;
    LPD3DXBUFFER pCode;      // Container for the compiled HLSL code



    //[ 1 ] DETERMINE FP TEXTURE SUPPORT
    //----------------------------------
    g_fmtHDR = D3DFMT_G32R32F;




    //[ 2 ] CREATE HDR RENDER TARGETS
    //-------------------------------
    int iTextureSize = 1;
    for( int i = 0; i < Luminance::g_dwLumTextures; i++ )
    {

        // Create this element in the array
        pDevice->CreateTexture( iTextureSize, iTextureSize, 1,
                                   D3DUSAGE_RENDERTARGET, Luminance::g_fmtHDR,
                                   D3DPOOL_DEFAULT, &Luminance::g_pTexLuminance[i], NULL );


        // Increment for the next texture
        iTextureSize *= 3;

    }



    //[ 3 ] CREATE GUI DISPLAY SHADER
    //-------------------------------
    // Because the intermediary luminance textures are stored as either G32R32F
    // or G16R16F, we need a pixel shader that can convert this to a more meaningful
    // greyscale ARGB value. This shader doesn't actually contribute to the
    // luminance calculations - just the way they are presented to the user via the GUI.

	string luminancePshFilePath = StringUtility::GetContentPath();
	luminancePshFilePath.append("shaders\\Luminance.psh");

    D3DXCompileShaderFromFile(
       luminancePshFilePath.c_str(),
       NULL, NULL,
       "LuminanceDisplay",
       "ps_2_0",
       0,
       &pCode,
       NULL,
       NULL
       );
    if( FAILED( hr ) )
    {

        // Couldn't compile the shader, use the 'compile_shaders.bat' script
        // in the 'Shader Code' folder to get a proper compile breakdown.
        OutputDebugString(
            "Luminance::CreateResources() - Compiling of 'LuminanceDisplay' from 'Luminance.psh' failed!\n" );
        return hr;

    }

    pDevice->CreatePixelShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ),
                                   &g_pLumDispPS );
    if( FAILED( hr ) )
    {

        // Couldn't turn the compiled shader into an actual, usable, pixel shader!
        OutputDebugString(
            "Luminance::CreateResources() - Could not create a pixel shader object for 'LuminanceDisplay'.\n" );
        pCode->Release();
        return hr;

    }

    pCode->Release();



    //[ 4 ] CREATE FIRST-PASS DOWNSAMPLE SHADER
    //-----------------------------------------
    // The first pass of down-sampling has to convert the RGB data to
    // a single luminance value before averaging over the kernel. This
	// is slightly different to the subsequent down-sampling shader.

    D3DXCompileShaderFromFile(
       luminancePshFilePath.c_str(),
       NULL, NULL,
       "GreyScaleDownSample",
       "ps_2_0",
       0,
       &pCode,
       NULL,
       &g_pLum1PSConsts
       ) ;
    if( FAILED( hr ) )
    {

        // Couldn't compile the shader, use the 'compile_shaders.bat' script
        // in the 'Shader Code' folder to get a proper compile breakdown.
        OutputDebugString(
            "Luminance::CreateResources() - Compiling of 'GreyScaleDownSample' from 'Luminance.psh' failed!\n" );
        return hr;

    }

    pDevice->CreatePixelShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ), &g_pLum1PS );
    if( FAILED( hr ) )
    {

        // Couldn't turn the compiled shader into an actual, usable, pixel shader!
        OutputDebugString(
            "Luminance::CreateResources() - Could not create a pixel shader object for 'GreyScaleDownSample'.\n" );
        pCode->Release();
        return hr;

    }

    pCode->Release();



    //[ 5 ] CREATE DOWNSAMPLING PIXEL SHADER
    //--------------------------------------
    // This down-sampling shader assumes that the incoming pixels are
    // already in G16R16F or G32R32F format, and of a paired luminance/maximum value.

    D3DXCompileShaderFromFile(
       luminancePshFilePath.c_str(),
       NULL, NULL,
       "DownSample",
       "ps_2_0",
       0,
       &pCode,
       NULL,
       &g_pLum3x3DSPSConsts
       ) ;
    if( FAILED( hr ) )
    {

        // Couldn't compile the shader, use the 'compile_shaders.bat' script
        // in the 'Shader Code' folder to get a proper compile breakdown.
        OutputDebugString( "Luminance::CreateResources() - Compiling of 'DownSample' from 'Luminance.psh' failed!\n"
                           );
        return hr;

    }

    pDevice->CreatePixelShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ),
                                   &g_pLum3x3DSPS );
    if( FAILED( hr ) )
    {

        // Couldn't turn the compiled shader into an actual, usable, pixel shader!
        OutputDebugString( "Luminance::CreateResources() : Could not create a pixel shader object for 'DownSample'.\n"
                           );
        pCode->Release();
        return hr;

    }

    pCode->Release();


    return hr;

}



//--------------------------------------------------------------------------------------
//  DestroyResources( )
//
//      DESC:
//          Makes sure that the resources acquired in CreateResources() are cleanly
//          destroyed to avoid any errors and/or memory leaks.
//
//--------------------------------------------------------------------------------------
HRESULT Luminance::DestroyResources()
{

    for( DWORD i = 0; i < g_dwLumTextures; i++ )
	{
        g_pTexLuminance[ i ]->Release();
	}

    g_pLumDispPS->Release();
    g_pLum1PS->Release();
    g_pLum1PSConsts->Release();
    g_pLum3x3DSPS->Release();
    g_pLum3x3DSPSConsts->Release();

    return S_OK;

}



//--------------------------------------------------------------------------------------
//  MeasureLuminance( )
//
//      DESC:
//          This is the core function for this particular part of the application, it's
//          job is to take the previously rendered (in the 'HDRScene' namespace) HDR
//          image and compute the overall luminance for the scene. This is done by
//          repeatedly downsampling the image until it is only 1x1 in size. Doing it
//          this way (pixel shaders and render targets) keeps as much of the work on
//          the GPU as possible, consequently avoiding any resource transfers, locking
//          and modification.
//
//      PARAMS:
//          pDevice : The currently active device that will be used for rendering.
//
//      NOTES:
//          The results from this function will eventually be used to compose the final
//          image. See OnFrameRender() in 'HDRDemo.cpp'.
//
//--------------------------------------------------------------------------------------
HRESULT Luminance::MeasureLuminance( IDirect3DDevice9* pDevice, HDRScene *hdrScene )
{

    //[ 0 ] DECLARE VARIABLES AND ALIASES
    //-----------------------------------
    LPDIRECT3DTEXTURE9 pSourceTex = NULL;     // We use this texture as the input
    LPDIRECT3DTEXTURE9 pDestTex = NULL;     // We render to this texture...
    LPDIRECT3DSURFACE9 pDestSurf = NULL;     // ... Using this ptr to it's top-level surface


    //[ 1 ] SET THE DEVICE TO RENDER TO THE HIGHEST
    //      RESOLUTION LUMINANCE MAP.
    //---------------------------------------------
    hdrScene->GetOutputTexture( &pSourceTex );
    pDestTex = g_pTexLuminance[ g_dwLumTextures - 1 ];
    if( FAILED( pDestTex->GetSurfaceLevel( 0, &pDestSurf ) ) )
    {

        // Couldn't acquire this surface level. Odd!
        OutputDebugString(
            "Luminance::MeasureLuminance( ) : Couldn't acquire surface level for hi-res luminance map!\n" );
        return E_FAIL;

    }

    pDevice->SetRenderTarget( 0, pDestSurf );
    pDevice->SetTexture( 0, pSourceTex );

    pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );


    //[ 2 ] RENDER AND DOWNSAMPLE THE HDR TEXTURE
    //      TO THE LUMINANCE MAP.
    //-------------------------------------------

    // Set which shader we're going to use. g_pLum1PS corresponds
    // to the 'GreyScaleDownSample' entry point in 'Luminance.psh'.
    pDevice->SetPixelShader( g_pLum1PS );

    // We need to compute the sampling offsets used for this pass.
    // A 2x2 sampling pattern is used, so we need to generate 4 offsets.
    //
    // NOTE: It is worth noting that some information will likely be lost
    //       due to the luminance map being less than 1/2 the size of the
    //       original render-target. This mis-match does not have a particularly
    //       big impact on the final luminance measurement. If necessary,
    //       the same process could be used - but with many more samples, so as
    //       to correctly map from HDR->Luminance without losing information.
    D3DXVECTOR4 offsets[4];

    // Find the dimensions for the source data
    D3DSURFACE_DESC srcDesc;
    pSourceTex->GetLevelDesc( 0, &srcDesc );

    // Because the source and destination are NOT the same sizes, we
    // need to provide offsets to correctly map between them.
    float sU = ( 1.0f / static_cast< float >( srcDesc.Width ) );
    float sV = ( 1.0f / static_cast< float >( srcDesc.Height ) );

    // The last two components (z,w) are unused. This makes for simpler code, but if
    // constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
    offsets[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
    offsets[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
    offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
    offsets[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

    // Set the offsets to the constant table
    g_pLum1PSConsts->SetVectorArray( pDevice, "tcLumOffsets", offsets, 4 );

    // With everything configured we can now render the first, initial, pass
    // to the luminance textures.
    RenderToTexture( pDevice );

    // Make sure we clean up the remaining reference
    pDestSurf->Release();
    pSourceTex->Release();


    //[ 3 ] SCALE EACH RENDER TARGET DOWN
    //      The results ("dest") of each pass feeds into the next ("src")
    //-------------------------------------------------------------------
    for( int i = ( g_dwLumTextures - 1 ); i > 0; i-- )
    {

        // Configure the render targets for this iteration
        pSourceTex = g_pTexLuminance[ i ];
        pDestTex = g_pTexLuminance[ i - 1 ];
        if( FAILED( pDestTex->GetSurfaceLevel( 0, &pDestSurf ) ) )
        {

            // Couldn't acquire this surface level. Odd!
            OutputDebugString( "Luminance::MeasureLuminance( ) : Couldn't acquire surface level for luminance map!\n"
                               );
            return E_FAIL;

        }

        pDevice->SetRenderTarget( 0, pDestSurf );
        pDevice->SetTexture( 0, pSourceTex );

        // We don't want any filtering for this pass
        pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
        pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

        // Because each of these textures is a factor of 3
        // different in dimension, we use a 3x3 set of sampling
        // points to downscale.
        D3DSURFACE_DESC srcTexDesc;
        pSourceTex->GetLevelDesc( 0, &srcTexDesc );

        // Create the 3x3 grid of offsets
        D3DXVECTOR4 DSoffsets[9];
        int idx = 0;
        for( int x = -1; x < 2; x++ )
        {
            for( int y = -1; y < 2; y++ )
            {
                DSoffsets[idx++] = D3DXVECTOR4(
                    static_cast< float >( x ) / static_cast< float >( srcTexDesc.Width ),
                    static_cast< float >( y ) / static_cast< float >( srcTexDesc.Height ),
                    0.0f,   //unused
                    0.0f    //unused
                    );
            }
        }

        // Set them to the current pixel shader
        pDevice->SetPixelShader( g_pLum3x3DSPS );
        g_pLum3x3DSPSConsts->SetVectorArray( pDevice, "tcDSOffsets", DSoffsets, 9 );

        // Render the display to this texture
        RenderToTexture( pDevice );

        // Clean-up by releasing the level-0 surface
        pDestSurf->Release();

    }


    // =============================================================
    //    At this point, the g_pTexLuminance[0] texture will contain
    //    a 1x1 texture that has the downsampled luminance for the
    //    scene as it has currently been rendered.
    // =============================================================

    return S_OK;

}

//--------------------------------------------------------------------------------------
//  GetLuminanceTexture( )
//
//      DESC:
//          The final 1x1 luminance texture created by the MeasureLuminance() function
//          is required as an input into the final image composition. Because of this
//          it is necessary for other parts of the application to have access to this
//          particular texture.
//
//      PARAMS:
//          pTexture    : Should be NULL on entry, will be a valid reference on exit
//
//      NOTES:
//          The code that requests the reference is responsible for releasing their
//          copy of the texture as soon as they are finished using it.
//
//--------------------------------------------------------------------------------------
HRESULT Luminance::GetLuminanceTexture( IDirect3DTexture9** pTex )
{

    // [ 0 ] ERASE ANY DATA IN THE INPUT
    //----------------------------------
	if ((*pTex) != NULL)
	{
		( *pTex )->Release();
	}

    // [ 1 ] COPY THE PRIVATE REFERENCE
    //---------------------------------
    *pTex = g_pTexLuminance[ 0 ];

    // [ 2 ] INCREMENT THE REFERENCE COUNT..
    //--------------------------------------
    ( *pTex )->AddRef();

    return S_OK;

}



//--------------------------------------------------------------------------------------
//  ComputeResourceUsage( )
//
//      DESC:
//          Based on the known resources this function attempts to make an accurate
//          measurement of how much VRAM is being used by this part of the application.
//
//      NOTES:
//          Whilst the return value should be pretty accurate, it shouldn't be relied
//          on due to the way drivers/hardware can allocate memory.
//
//          Only the first level of the render target is checked as there should, by
//          definition, be no mip levels.
//
//--------------------------------------------------------------------------------------
DWORD Luminance::ComputeResourceUsage()
{

    DWORD usage = 0;

    for( DWORD i = 0; i < g_dwLumTextures; i++ )
    {

        D3DSURFACE_DESC d;
        g_pTexLuminance[ i ]->GetLevelDesc( 0, &d );

        usage += ( ( d.Width * d.Height ) * ( g_fmtHDR == D3DFMT_G32R32F ? 8 : 4 ) );

    }

    return usage;

}



//--------------------------------------------------------------------------------------
//  RenderToTexture( )
//
//      DESC:
//          A simple utility function that draws, as a TL Quad, one texture to another
//          such that a pixel shader (configured before this function is called) can
//          operate on the texture. Used by MeasureLuminance() to perform the
//          downsampling and filtering.
//
//      PARAMS:
//          pDevice : The currently active device
//
//      NOTES:
//          n/a
//
//--------------------------------------------------------------------------------------
HRESULT RenderToTexture( IDirect3DDevice9* pDev )
{

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
    pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( TLVertex ) );

    return S_OK;

}




#include <d3d9.h>
#include <d3dx9.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../Global.h"
#include "../Utility/StringUtility.h"
#include "HDRScene.h"
#include "../Managers/SceneManager.h"
#include "../Managers/MeshManager.h"
#include "../Engine.h"
#include "../Camera.h"

struct LitVertex
{
    D3DXVECTOR3 p;
    DWORD c;
};

static const DWORD FVF_LITVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct TLVertex
{
    D3DXVECTOR4 p;
    D3DXVECTOR2 t;
};

static const DWORD FVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;

HDRScene::HDRScene()
{
	m_RenderedHDRTexture = NULL;
}

void HDRScene::LoadLightSphereMesh()
{
	string fullpath = StringUtility::GetContentPath();
	fullpath.append("sphere.x");
	LPD3DXBUFFER material;
	DWORD numMaterials = 0;
	m_LightSphereMesh = Engine::GetInstance()->GetMeshManager()->GetMesh(fullpath.c_str(), material, numMaterials);
}

HRESULT HDRScene::CreateResources( IDirect3DDevice9* pDevice, const D3DSURFACE_DESC* pDisplayDesc )
{
    HRESULT hr = S_OK;
    LPD3DXBUFFER pCode;

    pDevice->CreateTexture(pDisplayDesc->Width, pDisplayDesc->Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &m_RenderedHDRTexture, NULL);
	pDevice->CreateTexture(pDisplayDesc->Width, pDisplayDesc->Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &m_LightMapTexture, NULL);

	pDevice->CreateTexture(pDisplayDesc->Width, pDisplayDesc->Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_GBufferColorTexture, NULL);
	pDevice->CreateTexture(pDisplayDesc->Width, pDisplayDesc->Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_GBufferNormalTexture, NULL);
	pDevice->CreateTexture(pDisplayDesc->Width, pDisplayDesc->Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_GBufferDepthTexture, NULL);

	m_HalfPixel.x = 0.5f / pDisplayDesc->Width;
	m_HalfPixel.y = 0.5f / pDisplayDesc->Height;

	// GBuffer rendering Pixel Shader

	string gBufferShaderFilePath = StringUtility::GetContentPath();
	gBufferShaderFilePath.append("shaders\\RenderGBuffer.fx"); 
	D3DXCreateEffectFromFile( pDevice, gBufferShaderFilePath.c_str(), NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_GBufferEffect, NULL );

	// Final Combine rendering Pixel Shader

	string finalCombinePSFilePath = StringUtility::GetContentPath();
	finalCombinePSFilePath.append("shaders\\FinalCombine.psh"); 
	D3DXCompileShaderFromFile(finalCombinePSFilePath.c_str(), NULL, NULL, "main", "ps_3_0", 0, &pCode, NULL, &m_FinalCombinePSConsts);
	pDevice->CreatePixelShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ), &m_FinalCombinePS );
	pCode->Release();

	// Final Combine rendering Vertex Shader

	string finalCombineVSFilePath = StringUtility::GetContentPath();
	finalCombineVSFilePath.append("shaders\\FinalCombine.vsh");
	D3DXCompileShaderFromFile(finalCombineVSFilePath.c_str(), NULL, NULL, "main", "vs_3_0", 0, &pCode, NULL, &m_FinalCombineVSConsts );
	pDevice->CreateVertexShader( reinterpret_cast< DWORD* >( pCode->GetBufferPointer() ), &m_FinalCombineVS );
	pCode->Release();

	// Light rendering Pixel Shader

	string lightShaderFilePath = StringUtility::GetContentPath();
	lightShaderFilePath.append("shaders\\Light.fx"); 
	D3DXCreateEffectFromFile( pDevice, lightShaderFilePath.c_str(), NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_LightEffect, NULL );
	
	srand ( time(NULL) );

	for ( unsigned int i = 0; i < 200; i ++)
	{
		Light * light = new Light();

		float r = (float) rand()/RAND_MAX;
		float g = (float) rand()/RAND_MAX;
		float b = (float) rand()/RAND_MAX;

		float x = (float) rand()/RAND_MAX;
		float y = (float) rand()/RAND_MAX;
		float z = (float) rand()/RAND_MAX;

		light->color = D3DXVECTOR3(r, g, b);
		light->direction = D3DXVECTOR3(0,-1,0);
		light->position = D3DXVECTOR3( x * 60.0f - 30.0f, y * 10.0f + 5.0f, z * 60.0f - 30.0f);
		light->radius = 20.0f * (float) rand()/RAND_MAX + 1;
		light->intensity = 0.1f;

		m_Lights.push_back(light);
	}

    return hr;
}

HRESULT HDRScene::DestroyResources()
{
	m_LightEffect->Release();
	m_GBufferEffect->Release();

	m_RenderedHDRTexture->Release();
	m_LightMapTexture->Release();

	m_GBufferColorTexture->Release();
	m_GBufferNormalTexture->Release();
	m_GBufferDepthTexture->Release();

    return S_OK;
}

HRESULT HDRScene::RenderScene( IDirect3DDevice9* pDevice )
{
	// Grab the old surface (to restore when we are done)
    LPDIRECT3DSURFACE9 pPrevSurf = NULL;
    pDevice->GetRenderTarget( 0, &pPrevSurf );

    LPDIRECT3DSURFACE9 colorSurf = NULL;
	LPDIRECT3DSURFACE9 normalSurf = NULL;
	LPDIRECT3DSURFACE9 depthSurf = NULL;

	m_GBufferColorTexture->GetSurfaceLevel( 0, &colorSurf );
	m_GBufferNormalTexture->GetSurfaceLevel( 0, &normalSurf );
	m_GBufferDepthTexture->GetSurfaceLevel( 0, &depthSurf );

    pDevice->SetRenderTarget( 0, colorSurf );
	pDevice->SetRenderTarget( 1, normalSurf );
	pDevice->SetRenderTarget( 2, depthSurf );

	// Clear before draw
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );

	m_GBufferEffect->SetTechnique( "Technique1" );

	UINT iPass, cPasses;

	m_GBufferEffect->Begin( &cPasses, 0 );

	for( iPass = 0; iPass < cPasses; iPass++ )
	{
		m_GBufferEffect->BeginPass( iPass );

			SceneManager::GetInstance()->RenderGameObjects();

		m_GBufferEffect->EndPass();
	}

	m_GBufferEffect->End();

	// Switch back to the previous surface
    pDevice->SetRenderTarget( 0, pPrevSurf );
	pDevice->SetRenderTarget( 1, NULL );
	pDevice->SetRenderTarget( 2, NULL );

	DrawLights(pDevice);

	// Release our handles to the surfaces
    colorSurf->Release();
	normalSurf->Release();
	depthSurf->Release();
    pPrevSurf->Release();

    return S_OK;
}

void HDRScene::DrawLights(IDirect3DDevice9* pDevice)
{
	LPDIRECT3DSURFACE9 lightSurf = NULL;
	m_LightMapTexture->GetSurfaceLevel(0, &lightSurf);
	pDevice->SetRenderTarget(0, lightSurf);

	//clear all components to 0
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 );
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//use additive blending, and make sure the blending factors are as we need them
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//use the same operation on the alpha channel
	//GraphicsDevice.RenderState.SeparateAlphaBlendEnabled = false;
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	for (unsigned int i = 0; i < m_Lights.size(); i++)
	{
		DrawLight(pDevice, m_Lights[i]);
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	LPDIRECT3DSURFACE9 finalSurf = NULL;
	m_RenderedHDRTexture->GetSurfaceLevel(0, &finalSurf);
	pDevice->SetRenderTarget(0, finalSurf);

	//set the effect parameters
	pDevice->SetTexture( 0, m_GBufferColorTexture);
	pDevice->SetTexture( 1, m_LightMapTexture);
	m_FinalCombinePSConsts->SetFloatArray(pDevice, "halfPixel", m_HalfPixel, 2);

	pDevice->SetVertexShader( m_FinalCombineVS );
	pDevice->SetPixelShader( m_FinalCombinePS );

	FullScreenQuad(pDevice);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

void HDRScene::DrawLight(IDirect3DDevice9* pDevice, Light *light)
{

	// Compute the light world matrix, scale according to light radius, and translate it to light position
	D3DXMATRIX sphereScaleMatrix, sphereTranslateMatrix;
	D3DXMatrixScaling(&sphereScaleMatrix, light->radius, light->radius, light->radius);
	D3DXMatrixTranslation(&sphereTranslateMatrix, light->position.x, light->position.y, light->position.z);
	D3DXMATRIX sphereWorldMatrix = sphereScaleMatrix * sphereTranslateMatrix;

	D3DXMATRIX *view = Engine::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX *projection = Engine::GetInstance()->GetCamera()->GetProjectionMatrix();

	m_LightEffect->SetMatrix("World", &sphereWorldMatrix);
	m_LightEffect->SetMatrix("View", view);
	m_LightEffect->SetMatrix("Projection", projection);

	//light position
	D3DXVECTOR4 lightPosition(light->position.x, light->position.y, light->position.z, 0);
	m_LightEffect->SetVector("lightPosition", &lightPosition);

	//set the color, radius and Intensity
	D3DXVECTOR4 lightColor(light->color.x, light->color.y, light->color.z, 1);
	m_LightEffect->SetVector("Color", &lightColor);

	m_LightEffect->SetFloat("lightRadius", light->radius);
	m_LightEffect->SetFloat("lightIntensity", light->intensity);

	m_LightEffect->SetFloat("spotOuterCone", light->outerCone);
	m_LightEffect->SetFloat("spotInnerCone", light->innerCone);

	D3DXVECTOR4 lightDirection(light->direction.x, light->direction.y, light->direction.z, 0);
	m_LightEffect->SetVector("lightDirection", &lightDirection);

	//parameters for specular computations
	D3DXVECTOR3 *cameraPosition = Engine::GetInstance()->GetCamera()->GetCameraPosition();
	D3DXVECTOR4 cameraPositionVec4(cameraPosition->x, cameraPosition->y, cameraPosition->z, 0);
	m_LightEffect->SetVector("cameraPosition", &cameraPositionVec4);
	
	D3DXMATRIX cameraViewProjectionMatrix = (*view) * (*projection);
	D3DXMATRIX inverseCameraViewProjectionMatrix;
	D3DXMatrixInverse(&inverseCameraViewProjectionMatrix, NULL, &cameraViewProjectionMatrix);
	m_LightEffect->SetMatrix("InvertViewProjection", &inverseCameraViewProjectionMatrix);
	
	//size of a halfpixel, for texture coordinates alignment	
	m_LightEffect->SetFloatArray("halfPixel", m_HalfPixel, 2);

	D3DXVECTOR4 cameraDeltaLightPos = cameraPositionVec4 - lightPosition;
	//calculate the distance between the camera and light center
	float cameraToCenter = D3DXVec4Length(&cameraDeltaLightPos);
	//if we are inside the light volume, draw the sphere's inside face
	if (cameraToCenter < light->radius)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}
	else
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	//set the G-Buffer parameters
	m_LightEffect->SetTexture("colorMap", m_GBufferColorTexture);
	m_LightEffect->SetTexture("normalMap", m_GBufferNormalTexture);
	m_LightEffect->SetTexture("depthMap", m_GBufferDepthTexture);

	m_LightEffect->SetTechnique( "Technique1" );

	UINT iPass, cPasses;

	m_LightEffect->Begin( &cPasses, 0 );

		for( iPass = 0; iPass < cPasses; iPass++ )
		{
			m_LightEffect->BeginPass( iPass );

				m_LightSphereMesh->DrawSubset(0);

			m_LightEffect->EndPass();
		}

	m_LightEffect->End();

	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW) ;
}

HRESULT HDRScene::FullScreenQuad( IDirect3DDevice9* pDev )
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
	pDev->SetFVF( FVF_TLVERTEX );
	pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof( TLVertex ) );

	return S_OK;
}

HRESULT HDRScene::GetOutputTexture( IDirect3DTexture9** pTexture )
{
	if ((*pTexture) != NULL)
	{
		(*pTexture)->Release();
	}

    *pTexture = m_RenderedHDRTexture;

    ( *pTexture )->AddRef();

    return S_OK;
}








#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "ImprovedNoise.h"
#include "RenderTexture.h"
#include "Types.h"
#include "PhotonMap.h"
#include "NormalsGenerator.h"

using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 1024

#define NUM_VERTICES 600000

IDXGISwapChain *swapchain;
ID3D11Device *dev;
ID3D11DeviceContext *devcon;
ID3D11RenderTargetView *backbuffer;
ID3D11DepthStencilView *zbuffer;
ID3D11InputLayout *pLayout;

ID3D11VertexShader *pVS;
ID3D11PixelShader *pPS;
ID3D11GeometryShader *pGS;

ID3D11VertexShader *pDEFAULT_VS;
ID3D11PixelShader *pDEFAULT_PS;

ID3D11VertexShader *pDEBUG_VS;
ID3D11PixelShader *pDEBUG_PS;

ID3D11PixelShader *pSHADOW_PS;

ID3D11Buffer *pVBuffer;
ID3D11Buffer *pCBuffer;
ID3D11Buffer *pIBuffer;

ID3D11Buffer *floorVB;
ID3D11Buffer *floorIB;

ID3D11Buffer *fullScreenQuadVertexBuffer;
ID3D11Buffer *fullScreenQuadIndexBuffer;

ID3D11ShaderResourceView *smokeRV;

ID3D11RasterizerState *pRS;
ID3D11SamplerState *pSS;

ParticleVertex *vertices;
float *particleDistance;

ID3D11BlendState *pAdditiveBlendState;
ID3D11BlendState *pAlphaBlendState;

HWND windowHandle;

void InitD3D(HWND hWnd);
void RenderFrame();
void UpdateVertices();
void CleanD3D();
void InitGraphics();
void InitPipeline();
void InitStates();
void InitTextures();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

float randfloat() 
{ 
    return rand()/(float(RAND_MAX)+1); 
} 

float randfloat(float max) 
{ 
    return randfloat()*max; 
} 

float randfloat(float min, float max) 
{ 
    if (min>max) 
    { 
        return randfloat()*(min-max)+max;     
    } 
    else 
    { 
        return randfloat()*(max-min)+min; 
    }     
} 

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          "Volumetric Lighting Test",
                          WS_OVERLAPPEDWINDOW,
                          400,
                          50,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

	windowHandle = hWnd;

    ShowWindow(hWnd, nCmdShow);

    InitD3D(hWnd);

    MSG msg;

    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

		UpdateVertices();
        RenderFrame();
    }

    CleanD3D();

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

void InitFloorBuffers()
{
	GeometryVertex vertices[] =
    {
        {D3DXVECTOR3(-90.0f, 0.0f, -90.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3(-90.0f, 0.0f,  90.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3( 90.f,  0.0f, -90.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3( 90.0f, 0.0f,  90.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR3(0,1,0)},
    };

	DWORD indices[] = 
    {
        0, 1, 2,
        2, 1, 3
    };

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC; 
		bd.ByteWidth = sizeof(GeometryVertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &floorVB);

		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(floorVB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(GeometryVertex) * 4);
		devcon->Unmap(floorVB, NULL);
	}

	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(DWORD) * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		dev->CreateBuffer(&bd, NULL, &floorIB);

		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(floorIB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, indices, sizeof(DWORD) * 6);
		devcon->Unmap(floorIB, NULL);
	}
}

void DrawFloor()
{
	UINT stride = sizeof(GeometryVertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &floorVB, &stride, &offset);
	devcon->IASetIndexBuffer(floorIB, DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(6, 0, 0);
}

void InitFullScreenQuad()
{
	GeometryVertex vertices[] =
    {
        {D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3(-1.0f,  1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3( 1.f,  -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR3(0,1,0)},
        {D3DXVECTOR3( 1.0f,  1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR3(0,1,0)},
    };

	DWORD indices[] = 
    {
        0, 1, 2,
        2, 1, 3
    };

	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC; 
		bd.ByteWidth = sizeof(GeometryVertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bd, NULL, &fullScreenQuadVertexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(fullScreenQuadVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(GeometryVertex) * 4);
		devcon->Unmap(fullScreenQuadVertexBuffer, NULL);
	}

	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(DWORD) * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		dev->CreateBuffer(&bd, NULL, &fullScreenQuadIndexBuffer);

		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(fullScreenQuadIndexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, indices, sizeof(DWORD) * 6);
		devcon->Unmap(fullScreenQuadIndexBuffer, NULL);
	}
}

void DrawFullScreenQuad()
{
	UINT stride = sizeof(GeometryVertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &fullScreenQuadVertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(fullScreenQuadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(6, 0, 0);
}

void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 1;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL, // D3D11_CREATE_DEVICE_DEBUG,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);


    // create the depth buffer texture
    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = SCREEN_WIDTH;
    texd.Height = SCREEN_HEIGHT;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 1;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *pDepthBuffer;
    dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;

	ID3D11DepthStencilState* m_depthStencilState;

	dev->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	// Set the depth stencil state.
	devcon->OMSetDepthStencilState(m_depthStencilState, 1);

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
    pDepthBuffer->Release();

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;
    viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
    viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
	InitStates();
	InitTextures();
	InitFloorBuffers();
	InitFullScreenQuad();
}

D3DXVECTOR3 eye(-10, 20,20);
D3DXVECTOR3 light(20,30,20);
D3DXVECTOR3 at(0,10,0);
D3DXVECTOR3 up(0,1,0);

int compare (const void * a, const void * b)
{
	return ( ((ParticleVertex*)b)->distance - ((ParticleVertex*)a)->distance );
}

void UpdateVertices()
{
	for (int i = 0; i < NUM_VERTICES; i++)
	{
		//vertices[i].centerW.y += 0.1f;

		/*if (vertices[i].centerW.y > 25)
		{
			vertices[i].centerW.x = randfloat(-5, 5);
			vertices[i].centerW.y = randfloat(-5, 5);
			vertices[i].centerW.z = randfloat(-5, 5);
		}*/

		D3DXVECTOR3 delta = eye - D3DXVECTOR3(vertices[i].centerW.x, vertices[i].centerW.y, vertices[i].centerW.z);
		vertices[i].distance = D3DXVec3LengthSq(&delta);
	}

	// Sort vertices (for blending)

	qsort(vertices, NUM_VERTICES, sizeof(ParticleVertex), compare);
}

RenderTexture lightmaps[4];

float t = 0.0f;

void RenderFrame()
{
	t += 0.04f;
	light = D3DXVECTOR3(sin(t) * 20.0f, 30, cos(t) * 20.0f);

    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, vertices, sizeof(ParticleVertex) * NUM_VERTICES);
    devcon->Unmap(pVBuffer, NULL);

	ConstantBufferType constantBuffer;

	UINT stride = sizeof(ParticleVertex);
    UINT offset = 0;

	D3DXMATRIX viewMatrix, projectionMatrix;

	ID3D11RenderTargetView *views[4] = {lightmaps[0].GetRenderTargetView(), 
										lightmaps[1].GetRenderTargetView(), 
										lightmaps[2].GetRenderTargetView(),
										lightmaps[3].GetRenderTargetView()};
	devcon->OMSetRenderTargets(4, views, zbuffer);

	devcon->OMSetBlendState(pAdditiveBlendState, 0, 0xffffffff);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)D3DXToRadian(45), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 10000.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &light, &at, &up); 
	D3DXMatrixMultiply(&constantBuffer.gViewProj, &viewMatrix, &projectionMatrix);
	D3DXMatrixTranspose(&constantBuffer.gViewProj, &constantBuffer.gViewProj);
	constantBuffer.gLightViewProj = constantBuffer.gViewProj;
	constantBuffer.gEyePosW = light;

	lightmaps[0].ClearRenderTarget(devcon, zbuffer,0,0,0,0);
	lightmaps[1].ClearRenderTarget(devcon, zbuffer,0,0,0,0);
	lightmaps[2].ClearRenderTarget(devcon, zbuffer,0,0,0,0);
	lightmaps[3].ClearRenderTarget(devcon, zbuffer,0,0,0,0);
    //devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
    devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

	devcon->VSSetShader(pDEFAULT_VS, 0, 0);
    devcon->PSSetShader(pDEFAULT_PS, 0, 0);
	devcon->GSSetShader(NULL, 0, 0);

	devcon->UpdateSubresource(pCBuffer, 0, 0, &constantBuffer, 0, 0);

	//DrawFloor();

	devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pSHADOW_PS, 0, 0);
	devcon->GSSetShader(pGS, 0, 0);

    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
    devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    devcon->DrawIndexed(NUM_VERTICES, 0, 0);

	// Do final rendering

	devcon->OMSetBlendState(pAlphaBlendState, 0, 0xffffffff);

	devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);

    D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)D3DXToRadian(64.7), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 10000.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up); 
	D3DXMatrixMultiply(&constantBuffer.gViewProj, &viewMatrix, &projectionMatrix);
	D3DXMatrixTranspose(&constantBuffer.gViewProj, &constantBuffer.gViewProj);
	constantBuffer.gEyePosW = eye;
	constantBuffer.gLightPosW = light;

	devcon->UpdateSubresource(pCBuffer, 0, 0, &constantBuffer, 0, 0);

    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
    devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

	{
		ID3D11ShaderResourceView *tex[5] = {lightmaps[0].GetShaderResourceView(), 
											lightmaps[1].GetShaderResourceView(),
											lightmaps[2].GetShaderResourceView(),
											lightmaps[3].GetShaderResourceView(),
											smokeRV};
		devcon->PSSetShaderResources(0, 5, tex);
	}

	devcon->VSSetShader(pDEFAULT_VS, 0, 0);
    devcon->PSSetShader(pDEFAULT_PS, 0, 0);
	devcon->GSSetShader(NULL, 0, 0);

	DrawFloor();

	devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);
	devcon->GSSetShader(pGS, 0, 0);

    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
    devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    devcon->DrawIndexed(NUM_VERTICES, 0, 0);

	/*{ // DEBUG DRAWING FACILITY

		ID3D11ShaderResourceView *tex[2] = {lightmap.GetShaderResourceView(), smokeRV};
		devcon->PSSetShaderResources(0, 2, tex);

		devcon->VSSetShader(pDEBUG_VS, 0, 0);
		devcon->PSSetShader(pDEBUG_PS, 0, 0);
		devcon->GSSetShader(NULL, 0, 0);

		DrawFullScreenQuad();

	}*/

    swapchain->Present(0, 0);
}

void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);

    pLayout->Release();
    pVS->Release();
    pPS->Release();
    zbuffer->Release();
    pVBuffer->Release();
    pCBuffer->Release();
    pIBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}

//#define PHOTON_LIGHTING 0

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;

	found = str.find_first_of(separator);

	while(found != string::npos)
	{
		if(found > 0)
		{
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}

	if(str.length() > 0)
	{
		results->push_back(str);
	}
}

void InitGraphics()
{
    vertices = new ParticleVertex[NUM_VERTICES];

	{
		FILE *file = fopen ( "C:\\Users\\Richard\\Documents\\Visual Studio 2010\\Projects\\OpacityShadowMaps\\OpacityShadowMaps\\particleData.pdt", "r" );

		if ( file != NULL )
		{
			char line[256];

			int i = 0;

			while ( fgets( line, sizeof line, file ) != NULL && i < NUM_VERTICES)
			{
				vector<string> shrapnel;

				StringExplode(line, ",", &shrapnel);

				if (shrapnel.size() >= 3)
				{
					vertices[i].centerW.x = atof(shrapnel[0].c_str());
					vertices[i].centerW.y = atof(shrapnel[1].c_str());
					vertices[i].centerW.z = atof(shrapnel[2].c_str());
					vertices[i].centerW.w = 1.0f;
				}

				i++;
			}
		}
		else
		{
			cout << "COULD NOT LOAD MODEL\n";
		}
	}

	/*NormalsGenerator *generator = new NormalsGenerator(NUM_VERTICES);

	for (int i = 0; i < NUM_VERTICES; i++)
	{
		float pos[3] = {vertices[i].centerW.x, vertices[i].centerW.y, vertices[i].centerW.z};
		generator->store(i, pos);
	}

	generator->balance();*/

	particleDistance = new float[NUM_VERTICES];

	PhotonMap *map = new PhotonMap(NUM_VERTICES);

	for (int i = 0; i < NUM_VERTICES; i++)
	{
		//vertices[i].centerW.x = randfloat(-5, 5);
		//vertices[i].centerW.y = randfloat(0, 10);
		//vertices[i].centerW.z = randfloat(-5, 5);
		vertices[i].centerW.w = 1.0f;
		vertices[i].sizeW = D3DXVECTOR4(0.2f, 0.2f, 0, 0);
		vertices[i].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.1f);//D3DXVECTOR4((vertices[i].centerW.x+5)*0.1, (vertices[i].centerW.y+5)*0.1, (vertices[i].centerW.z+5)*0.1, .05);//
		
		float normal[3];
		float pos[3] = {vertices[i].centerW.x, vertices[i].centerW.y, vertices[i].centerW.z};

		//generator->normal_estimate(normal, pos, 0.1f, 100.0f);

		D3DXVECTOR3 n(normal[0], normal[1], normal[2]);
		D3DXVECTOR3 n_unit;
		D3DXVec3Normalize(&n_unit, &n);
		
		vertices[i].normal = D3DXVECTOR4(n_unit, 1.0) * -1;//D3DXVECTOR4(randfloat(-1,1), randfloat(-1,1), randfloat(-1,1), 0);
#ifdef PHOTON_LIGHTING
		float posl[3] = {vertices[i].centerW.x, vertices[i].centerW.y, vertices[i].centerW.z};
		map->store(i, posl);
#endif
	}

#ifdef PHOTON_LIGHTING

	map->balance();

	int nphotons = 10;

	NearestPhotons np;
	np.dist2 = (float*)malloc( sizeof(float)*(nphotons+1) );
	np.index = (Photon**)malloc( sizeof(Photon*)*(nphotons+1) );

	for (int i = 0; i < 100000; i++)
	{
		D3DXVECTOR3 d(randfloat(-1,1), randfloat(-1,0), randfloat(-1,1));
		D3DXVECTOR3 dir;
		D3DXVec3Normalize(&dir, &d);
		D3DXVECTOR3 pos(10,10,10);
		float power = 2.0f;

		int j = 0;
		while (j < 1000 && power > 0.001f)
		{
			pos += dir * 0.1;

			//int nphotons = 10;
			int max_dist = 1;

			//NearestPhotons np;
			//np.dist2 = (float*)malloc( sizeof(float)*(nphotons+1) );
			//np.index = (Photon**)malloc( sizeof(Photon*)*(nphotons+1) );

			np.pos[0] = pos[0]; np.pos[1] = pos[1]; np.pos[2] = pos[2];
			np.max = nphotons;
			np.found = 0;
			np.got_heap = 0;
			np.dist2[0] = max_dist*max_dist;

			// locate the nearest photons
			map->locate_photons( &np, 1 );

			for (int k = 1; k <= np.found; k++)
			{
				np.index[k]->power += power;
			}

			if (np.found > 0)
			{
				power -= 0.01f;
			}

			//free(np.dist2);
			//free(np.index);

			j++;
		}
	}

	free(np.dist2);
	free(np.index);

	Photon *lightData = map->get_photons();

	for (int i = 1; i <= map->get_stored_photons(); i++)
	{
		vertices[lightData[i].vertIndex].color.x *= lightData[i].power;
		vertices[lightData[i].vertIndex].color.y *= lightData[i].power;
		vertices[lightData[i].vertIndex].color.z *= lightData[i].power;
	}

#endif

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ParticleVertex) * NUM_VERTICES;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&bd, NULL, &pVBuffer);

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, vertices, sizeof(ParticleVertex) * NUM_VERTICES);  // copy the data
    devcon->Unmap(pVBuffer, NULL);

    // create the index buffer out of DWORDs
    DWORD *OurIndices = new DWORD[NUM_VERTICES];

	for (int i = 0; i < NUM_VERTICES; i++)
	{
		OurIndices[i] = i;
	}

    // create the index buffer
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(DWORD) * NUM_VERTICES;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;

    dev->CreateBuffer(&bd, NULL, &pIBuffer);

    devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurIndices, sizeof(DWORD) * NUM_VERTICES);         // copy the data
    devcon->Unmap(pIBuffer, NULL);
}

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, CHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	std::string str = compileErrors;
	OutputDebugString(str.c_str());

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	//MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void InitPipeline()
{
    ID3D10Blob *VS, *PS, *GS, *DEFAULT_VS, *DEFAULT_PS, *SHADOW_PS, *DEBUG_PS, *DEBUG_VS;

	ID3D10Blob *errorbuffer = NULL;

    D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS, 0, 0);
	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "GS", "gs_4_0", 0, 0, 0, &GS, 0, 0);

	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "DEFAULT_VS", "vs_4_0", 0, 0, 0, &DEFAULT_VS, 0, 0);
	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "DEFAULT_PS", "ps_4_0", 0, 0, 0, &DEFAULT_PS, 0, 0);

	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "DEBUG_VS", "vs_4_0", 0, 0, 0, &DEBUG_VS, 0, 0);
	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "DEBUG_PS", "ps_4_0", 0, 0, 0, &DEBUG_PS, 0, 0);

	D3DX11CompileFromFile("..\\OpacityShadowMaps\\shaders.hlsl", 0, 0, "SHADOW_PS", "ps_4_0", 0, 0, 0, &SHADOW_PS,  &errorbuffer, 0);

	OutputShaderErrorMessage(errorbuffer, windowHandle, "shaders.hlsl");

    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	dev->CreateGeometryShader(GS->GetBufferPointer(), GS->GetBufferSize(), NULL, &pGS);

 	dev->CreateVertexShader(DEFAULT_VS->GetBufferPointer(), DEFAULT_VS->GetBufferSize(), NULL, &pDEFAULT_VS);
 	dev->CreatePixelShader(DEFAULT_PS->GetBufferPointer(), DEFAULT_PS->GetBufferSize(), NULL, &pDEFAULT_PS);

	dev->CreateVertexShader(DEBUG_VS->GetBufferPointer(), DEBUG_VS->GetBufferSize(), NULL, &pDEBUG_VS);
 	dev->CreatePixelShader(DEBUG_PS->GetBufferPointer(), DEBUG_PS->GetBufferSize(), NULL, &pDEBUG_PS);

	dev->CreatePixelShader(SHADOW_PS->GetBufferPointer(), SHADOW_PS->GetBufferSize(), NULL, &pSHADOW_PS);

    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);
	devcon->GSSetShader(pGS, 0, 0);

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"SIZE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 4, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferType); // 64 * 6; //
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);
    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	devcon->PSSetConstantBuffers(0, 1, &pCBuffer);
	devcon->GSSetConstantBuffers(0, 1, &pCBuffer);

	lightmaps[0].Initialize(dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	lightmaps[1].Initialize(dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	lightmaps[2].Initialize(dev, SCREEN_WIDTH, SCREEN_HEIGHT);
	lightmaps[3].Initialize(dev, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void InitTextures()
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(dev, "smoke.jpg", 0, 0, &smokeRV, 0);

	if (FAILED(result))
	{
		OutputDebugString("Failed to load texture");
	}

	devcon->PSSetShaderResources(0, 1, &smokeRV);
}

// initializes the states
void InitStates()
{
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.MultisampleEnable = TRUE;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 0.0f;

    dev->CreateRasterizerState(&rd, &pRS);

    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    dev->CreateSamplerState(&sd, &pSS);

    D3D11_BLEND_DESC additiveBlendStateDesc;
    
	additiveBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	additiveBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	additiveBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additiveBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additiveBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	additiveBlendStateDesc.IndependentBlendEnable = FALSE;
    additiveBlendStateDesc.AlphaToCoverageEnable = FALSE;

	dev->CreateBlendState(&additiveBlendStateDesc, &pAdditiveBlendState);

	D3D11_BLEND_DESC alphaBlendStateDesc;
    
	alphaBlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	alphaBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	alphaBlendStateDesc.IndependentBlendEnable = FALSE;
    alphaBlendStateDesc.AlphaToCoverageEnable = FALSE;

    dev->CreateBlendState(&alphaBlendStateDesc, &pAlphaBlendState);
}
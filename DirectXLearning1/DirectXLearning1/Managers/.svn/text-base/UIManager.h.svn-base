#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class UIManager
{
public:

	static UIManager* GetInstance();
	~UIManager();

	void Initialize(LPDIRECT3DDEVICE9 direct3DDevice);
	void Deinitialize();

	void Draw();
	void Update();

	LPD3DXFONT GetFont();


protected:

	UIManager();

private:

	LPD3DXFONT m_Font;

	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	static UIManager* pInstance;
};

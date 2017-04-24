#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class MenuEntry;
class UIManager;

class Menu
{
public:

	Menu(D3DXVECTOR2 position, LPDIRECT3DDEVICE9 direct3DDevice, UIManager *parent);
	~Menu();

	void Draw();
	void Update();

	void Activate();

	MenuEntry * AddMenuEntry(const char* label, unsigned int actionCode);

	void GetPosition(D3DXVECTOR2 &position);

	LPD3DXFONT GetFont();

private:

	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	D3DXVECTOR2 m_Position;

	vector <MenuEntry*> m_MenuEntries;

	UIManager *m_Parent;
};
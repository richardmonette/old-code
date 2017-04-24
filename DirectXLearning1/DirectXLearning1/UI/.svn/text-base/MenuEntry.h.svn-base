#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class Menu;
class UIManager;

class MenuEntry
{
public:

	MenuEntry(unsigned int index, const char *label, Menu *parent, unsigned int actionCode);
	~MenuEntry();

	void Draw();
	void Update();
	void Activate();

private:

	unsigned int m_Index;
	unsigned int m_ActionCode;

	string m_Label;

	bool m_Active;

	RECT m_Rect;

	Menu *m_Parent;
};
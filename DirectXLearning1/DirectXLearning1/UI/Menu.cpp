#include "Menu.h"
#include "MenuEntry.h"
#include "../Managers/UIManager.h"

Menu::Menu(D3DXVECTOR2 position, LPDIRECT3DDEVICE9 direct3DDevice, UIManager *parent)
{
	m_Position = position;
	m_Direct3DDevice = direct3DDevice;
	m_Parent = parent;
}

Menu::~Menu()
{
}

void Menu::Draw()
{
	for (unsigned int i = 0; i < m_MenuEntries.size(); i++)
	{
		m_MenuEntries[i]->Draw();
	}
}

void Menu::Update()
{
	for (unsigned int i = 0; i < m_MenuEntries.size(); i++)
	{
		m_MenuEntries[i]->Update();
	}
}

MenuEntry * Menu::AddMenuEntry(const char* label, unsigned int actionCode)
{
	MenuEntry *menuEntry = new MenuEntry(m_MenuEntries.size(), label, this, actionCode);
	m_MenuEntries.push_back(menuEntry);

	return menuEntry;
}

void Menu::Activate()
{
	for (unsigned int i = 0; i < m_MenuEntries.size(); i++)
	{
		m_MenuEntries[i]->Activate();
	}
}

LPD3DXFONT Menu::GetFont()
{
	return m_Parent->GetFont();
}

void Menu::GetPosition(D3DXVECTOR2 &position)
{
	position = m_Position;
}
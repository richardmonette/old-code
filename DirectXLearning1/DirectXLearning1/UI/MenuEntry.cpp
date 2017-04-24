#include "MenuEntry.h"
#include "Menu.h"
#include "../Managers/InputManager.h"
#include "../Managers/UIManager.h"

MenuEntry::MenuEntry(unsigned int index, const char *label, Menu *parent, unsigned int actionCode)
{
	m_Index = index;
	m_Label = label;
	m_Parent = parent;
	m_ActionCode = actionCode;
}

MenuEntry::~MenuEntry()
{
}

void MenuEntry::Draw()
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 100, 100, 100);

	if (m_Active)
	{
		fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
	
	D3DXVECTOR2 parentPosition;
	m_Parent->GetPosition(parentPosition);

	m_Rect.left = (long)parentPosition.x;
	m_Rect.right = (long)parentPosition.x + 200;
	m_Rect.top = (long)parentPosition.y + m_Index * 10;
	m_Rect.bottom = m_Rect.top + 10;

	ostringstream oss;
	oss << m_Label;

	//TODO: Get the font into the UI manager as a singleton so it can be shared around

	UIManager::GetInstance()->GetFont()->DrawText(NULL, oss.str().c_str(), -1, &m_Rect, 0, fontColor );
}

void MenuEntry::Activate()
{
	if (m_Active)
	{
		//UIManager::GetInstance()->HandleActionCode(m_ActionCode, "Content/StandardCube.x");
	}
}

void MenuEntry::Update()
{
	D3DXVECTOR2 cursorPosition;
	InputManager::GetInstance()->GetCursorPosition(cursorPosition);

	if (cursorPosition.x < m_Rect.right && cursorPosition.x > m_Rect.left 
		&& cursorPosition.y > m_Rect.top && cursorPosition.y < m_Rect.bottom
		)
	{
		m_Active = true;
	}
	else
	{
		m_Active = false;
	}
}

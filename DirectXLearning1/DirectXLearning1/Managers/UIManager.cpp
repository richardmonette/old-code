#include "UIManager.h"
#include "InputManager.h"
#include "SceneManager.h"

#include "../UI/Menu.h"

#include "../Engine.h"
#include "../Camera.h"

static const float OBJECT_CREATE_DISTANCE = 5.0f;

UIManager* UIManager::pInstance = NULL;

UIManager* UIManager::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new UIManager;
	}

	return pInstance;
}

UIManager::UIManager()
{
}

void UIManager::Initialize(LPDIRECT3DDEVICE9 direct3DDevice)
{
	m_Direct3DDevice = direct3DDevice;

	D3DXCreateFont( m_Direct3DDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Anivers", &m_Font );
}

void UIManager::Deinitialize()
{
	m_Font->Release();
}

UIManager::~UIManager()
{
}

void UIManager::Draw()
{
}

void UIManager::Update()
{
}

LPD3DXFONT UIManager::GetFont()
{
	return m_Font;
}

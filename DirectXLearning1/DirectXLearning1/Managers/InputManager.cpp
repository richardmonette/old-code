#include "InputManager.h"
#include "assert.h"
#include "../Utility/StringUtility.h"
#include "../Objects/GameObject.h"
#include "../Objects/Handle.h"
#include "SceneManager.h"

InputManager* InputManager::pInstance = NULL; 

InputManager* InputManager::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new InputManager;
	}

	return pInstance;
}

InputManager::InputManager()
{
	m_CursorPosition = D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

	m_CursorTexture = NULL;
	m_IsMouseLocked = true;
}

InputManager::~InputManager()
{
}

void InputManager::Render()
{
	//TODO: Use the generic sprite batch from the engine (call a drawsprite method)
	m_CursorSprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 position(m_CursorPosition.x, m_CursorPosition.y, 0.0f);
	m_CursorSprite->Draw(m_CursorTexture, NULL, &center, &position, D3DCOLOR_XRGB(255, 255, 255));

	m_CursorSprite->End();
}

void InputManager::Update()
{
	DetectKeys();
	DetectMouse();

	m_CursorPosition.x += m_CurrentMouseState.lX;
	m_CursorPosition.y += m_CurrentMouseState.lY;

	m_CursorDelta.x = (float)m_CurrentMouseState.lX;
	m_CursorDelta.y = (float)m_CurrentMouseState.lY;

	if (m_CursorPosition.x < 0.0f)
	{
		m_CursorPosition.x = 0.0f;
	}

	if (m_CursorPosition.y < 0.0f)
	{
		m_CursorPosition.y = 0.0f;
	}

	if (m_CursorPosition.x > SCREEN_WIDTH)
	{
		m_CursorPosition.x = SCREEN_WIDTH;
	}

	if (m_CursorPosition.y > SCREEN_HEIGHT)
	{
		m_CursorPosition.y = SCREEN_HEIGHT;
	}

	if (m_IsMouseLocked)
	{
		SetCursorPos((int)SCREEN_WIDTH/2, (int)SCREEN_HEIGHT/2);
	}

	if (MouseButtonUp(InputManager::LEFT_MOUSE_BUTTON))
	{
		ostringstream oss;
		oss << "MouseButtonUpon " << "\n";

		OutputDebugStr(oss.str().c_str());

		SceneManager::GetInstance()->MouseUpObjects();
	}
	else if (MouseButtonDown(InputManager::LEFT_MOUSE_BUTTON))
	{
		GameObject *gameObject = SceneManager::GetInstance()->PickGameObject();

		if (gameObject != NULL)
		{
			gameObject->HandleMouseDown();

			ostringstream oss;
			oss << "MouseButtonDown on " << gameObject->GetName() << "\n";

			OutputDebugStr(oss.str().c_str());

			if (gameObject->GetSelectable() == true)
			{
				SceneManager::GetInstance()->SetSelectedObject(gameObject);
			}
		}
	}
}

void InputManager::Initialize(HINSTANCE hInstance, HWND hWnd, LPDIRECT3DDEVICE9 direct3DDevice)
{
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);

	m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DirectInputKeyboard, NULL);

	m_DirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_DirectInputKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	m_DirectInput->CreateDevice(GUID_SysMouse, &m_DirectInputMouse, NULL);

	m_DirectInputMouse->SetDataFormat(&c_dfDIMouse);
	m_DirectInputMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	m_DirectInputMouse->Acquire();

	Update();

	m_Direct3DDevice = direct3DDevice;

	D3DXCreateSprite(m_Direct3DDevice, &m_CursorSprite);
	string cursorTexturePath = StringUtility::GetContentPath();
	cursorTexturePath.append("cursor.png");
	D3DXCreateTextureFromFile(m_Direct3DDevice, cursorTexturePath.c_str(), &m_CursorTexture);

	assert(m_CursorTexture);

	return;
}

void InputManager::ToggleMouseLock()
{
	m_IsMouseLocked = !m_IsMouseLocked;
}

bool InputManager::IsKeyClicked(int key)
{
	if (!IsKeyPressed(key))
	{
		if(m_PreviousKeyState[key] & 0x80)
		{
			return true;
		}
	}

	return false;
}

bool InputManager::IsKeyPressed(int key)
{
	if(m_CurrentKeyState[key] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::MouseButtonUp(unsigned int index)
{
	if (!IsMouseButtonPressed(index))
	{
		if (m_PreviousMouseState.rgbButtons[index] & 0x80)
		{
			return true;
		}
	}

	return false;
}

bool InputManager::MouseButtonDown(unsigned int index)
{
	if (IsMouseButtonPressed(index))
	{
		if (m_PreviousMouseState.rgbButtons[index] & 0x80)
		{
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool InputManager::IsMouseButtonPressed(unsigned int index)
{
	if (m_CurrentMouseState.rgbButtons[index] & 0x80)
	{
		return true;
	}
	
	return false;
}

void InputManager::GetCursorDelta(D3DXVECTOR2 & cursorDelta)
{
	cursorDelta.x = m_CursorDelta.x;
	cursorDelta.y = m_CursorDelta.y;
}

void InputManager::GetCursorPosition(D3DXVECTOR2 &cursorPosition)
{
	cursorPosition.x = m_CursorPosition.x;
	cursorPosition.y = m_CursorPosition.y;
}

DIMOUSESTATE * InputManager::GetMouseState()
{
	return &m_CurrentMouseState;
}

void InputManager::DetectKeys()
{
	memcpy(m_PreviousKeyState, m_CurrentKeyState, 256);

	m_DirectInputKeyboard->Acquire();
	m_DirectInputKeyboard->GetDeviceState(256, (LPVOID)m_CurrentKeyState);

	return;
}

void InputManager::DetectMouse()
{
	m_PreviousMouseState = m_CurrentMouseState;

	m_DirectInputMouse->Acquire();
	m_DirectInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_CurrentMouseState);

	return;
} 

void InputManager::Deinitialize()
{
	m_CursorSprite->Release();
	m_CursorTexture->Release();
	m_DirectInputKeyboard->Unacquire();
	m_DirectInputMouse->Unacquire();
	m_DirectInput->Release();

	return;
}

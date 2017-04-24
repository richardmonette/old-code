#pragma once

#include "../Global.h"
#include <dinput.h>

#include <d3d9.h>
#include <d3dx9.h>

class InputManager
{
public:

	enum
	{
		LEFT_MOUSE_BUTTON = 0,
		RIGHT_MOUSE_BUTTON = 1
	};

	static InputManager* GetInstance();
	virtual ~InputManager();

	void Initialize(HINSTANCE hInstance, HWND hWnd, LPDIRECT3DDEVICE9 direct3DDevice);
	void Deinitialize();

	void Update();
	void Render();
	
	bool IsKeyPressed(int key);
	bool IsKeyClicked(int key);

	void ToggleMouseLock();

	DIMOUSESTATE * GetMouseState();

	void GetCursorPosition(D3DXVECTOR2 &cursorPosition);
	void GetCursorDelta(D3DXVECTOR2 & cursorDelta);

protected:

	InputManager();

private:

	bool IsMouseButtonPressed(unsigned int index);

	bool MouseButtonUp(unsigned int index);
	bool MouseButtonDown(unsigned int index);

	bool m_IsMouseLocked;

	LPDIRECTINPUT8 m_DirectInput;
	LPDIRECTINPUTDEVICE8 m_DirectInputKeyboard;
	LPDIRECTINPUTDEVICE8 m_DirectInputMouse;

	unsigned char m_CurrentKeyState[256];
	unsigned char m_PreviousKeyState[256];

	DIMOUSESTATE m_CurrentMouseState;
	DIMOUSESTATE m_PreviousMouseState;

	D3DXVECTOR2 m_CursorPosition;
	D3DXVECTOR2 m_CursorDelta;

	void DetectKeys();
	void DetectMouse();

	LPD3DXSPRITE m_CursorSprite;
	LPDIRECT3DTEXTURE9 m_CursorTexture;

	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	static InputManager* pInstance;

};

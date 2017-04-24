//=====================================
// Everything here is based on:
// http://rastertek.com/dx11tut13.html
//=====================================

#include "Input.h"

Input::Input()
{
	directInput = 0;
	keyboard = 0;
	mouse = 0;
}

Input::~Input()
{
		// Release the mouse.
	if(mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}

	// Release the keyboard.
	if(keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	// Release the main interface to direct input.
	if(directInput)
	{
		directInput->Release();
		directInput = 0;
	}

	ShowCursor(true);

	return;
}

Input::Input(HINSTANCE hinstance, HWND hwnd, int w, int h)
{
	screenWidth = w;
	screenHeight = h;

	mouseX = 0;
	mouseY = 0;

	DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);

	// Set up the keyboard
	directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();

	// Set up the mouse
	directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	mouse->Acquire();

	ShowCursor(false);
}

int Input::GetMouseX()
{
	return mouseX;
}

int Input::GetMouseY()
{
	return mouseY;
}

bool Input::LeftMouseDown()
{
	if (mouseState.rgbButtons[0] & 0x80)
		return true;
	return false;
}


bool Input::Wdown()
{
	if ( keyboardState[DIK_W] & 0x80)
		return true;
	return false;
}

bool Input::Adown()
{	
	if ( keyboardState[DIK_A] & 0x80)
		return true;
	return false;
}

bool Input::Sdown()
{
	if ( keyboardState[DIK_S] & 0x80)
		return true;
	return false;
}

bool Input::Ddown()
{
	if ( keyboardState[DIK_D] & 0x80)
		return true;
	return false;
}

void Input::Frame()
{
	ReadKeyboard();
	ReadMouse();

	mouseX = mouseState.lX;
	mouseY = mouseState.lY;

	SetCursorPos(screenWidth / 2, screenHeight / 2);
}

void Input::ReadKeyboard()
{
	HRESULT result;

	result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		keyboard->Acquire();
}

void Input::ReadMouse()
{
	HRESULT result;

	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		mouse->Acquire();
}

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <windows.h>

class Input
{
public:
	Input();
	Input(HINSTANCE hinstance, HWND hwnd, int w, int h);
	~Input();
	void Input::Frame();
	char * Input::GetKeyboardState();
	int Input::GetMouseX();
	int Input::GetMouseY();
	bool Input::LeftMouseDown();
	bool Input::Wdown();
	bool Input::Adown();
	bool Input::Sdown();
	bool Input::Ddown();

private:
	void Input::ReadKeyboard();
	void Input::ReadMouse();
	void Input::ProcessInput();

	IDirectInput8 *			directInput;
	IDirectInputDevice8 *	keyboard;
	IDirectInputDevice8 *	mouse;
	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;
	int screenWidth, screenHeight;
	int mouseX, mouseY;
};
#include "Global.h"
#include "Engine.h"
#include <AntTweakBar.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, "WindowClass", "Engine", WS_OVERLAPPEDWINDOW, 200, 200, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    MSG msg;

	Engine::GetInstance()->Initialize(hWnd, hInstance);

	bool continueUpdating = true;

	//ShowCursor(false);

    while(continueUpdating)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

        continueUpdating = Engine::GetInstance()->Update();

		Engine::GetInstance()->Render();

        if(!continueUpdating)
		{
            PostMessage(hWnd, WM_DESTROY, 0, 0);
		}
    }

	Engine::GetInstance()->Deinitialize();

	ShowCursor(true);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

	TwEventWin(hWnd, message, wParam, lParam);

    return DefWindowProc (hWnd, message, wParam, lParam);
}
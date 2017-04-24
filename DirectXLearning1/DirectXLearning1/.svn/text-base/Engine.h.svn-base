#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Camera;
class InputManager;
class SceneManager;
class TextureManager;
class MeshManager;
class LuaManager;
class UIManager;

class Engine
{
public:

	static Engine* GetInstance();
	~Engine();

	void Initialize(HWND hWnd, HINSTANCE hInstance);
	void Deinitialize();

	bool Update();
	void Render();

	Camera * GetCamera();
	MeshManager * GetMeshManager();

protected:

	Engine();

private:

	void LoadLevel();

	Camera *camera;

	TextureManager *textureManager;
	MeshManager *modelManager;
	LuaManager *luaManager;

	static Engine* pInstance;
};

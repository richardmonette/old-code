#pragma once

#include "../Global.h"

#include <d3d9.h>
#include <d3dx9.h>

class GameObject;
class MeshManager;
class Handle;

class SceneManager
{
public:
	
	static SceneManager* GetInstance();
	~SceneManager();

	void Initialize(LPDIRECT3DDEVICE9 direct3DDevice, MeshManager *modelManager);
	void Deinitialize();

	void Update();

	void RenderGameObjects();

	void SetSelectedObject(GameObject *gameObject);

	void SaveSceneToFile(const char *filename);
	void LoadSceneFromFile(const char *filename);

	GameObject * AddGameObject(const char *filename, D3DXVECTOR3 position, const char *name, bool isStatic);
	void AddGameObject(GameObject *gameObject);

	void MouseUpObjects();
	GameObject * PickGameObject();
	void CreateHandles();

protected:

	SceneManager();

private:

	LPDIRECT3DDEVICE9 m_Direct3DDevice;

	MeshManager *m_meshManager;

	vector<GameObject*> m_GameObjects;

	Handle *m_XHandle;
	Handle *m_YHandle;
	Handle *m_ZHandle;

	GameObject *m_Selected;

	static SceneManager* pInstance;
};

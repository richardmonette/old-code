#define NOMINMAX // for PhysX

#include "Engine.h"
#include "Camera.h"
#include "Objects/GameObject.h"
#include "Managers/InputManager.h"
#include "Managers/SceneManager.h"
#include "Managers/MeshManager.h"
#include "Managers/TextureManager.h"
#include "Managers/UIManager.h"
#include "Render/HDRPipeline.h"
#include "UI/Menu.h"
#include "Utility/StringUtility.h"
#include <AntTweakBar.h>

Engine* Engine::pInstance = NULL;

Engine* Engine::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new Engine;
	}

	return pInstance;
}

Engine::Engine()
{
}

void Engine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	HDRPipeline::GetInstance()->Initialize(hWnd, hInstance);

	TwInit(TW_DIRECT3D9, HDRPipeline::GetInstance()->GetDirect3DDevice());
	TwWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	PhysicsManager::GetInstance()->Initialize();

	camera = new Camera();

	InputManager::GetInstance()->Initialize(hInstance, hWnd, HDRPipeline::GetInstance()->GetDirect3DDevice());
	UIManager::GetInstance()->Initialize(HDRPipeline::GetInstance()->GetDirect3DDevice());
	modelManager = new MeshManager(HDRPipeline::GetInstance()->GetDirect3DDevice());
	HDRPipeline::GetInstance()->GetHDRScene()->LoadLightSphereMesh();
	SceneManager::GetInstance()->Initialize(HDRPipeline::GetInstance()->GetDirect3DDevice(), modelManager);
	textureManager = new TextureManager();

	LoadLevel();
}

void Engine::Deinitialize()
{
	delete camera;

	PhysicsManager::GetInstance()->Deinitialize();

	InputManager::GetInstance()->Deinitialize();
	UIManager::GetInstance()->Deinitialize();
	SceneManager::GetInstance()->Deinitialize();

	delete textureManager;
	delete modelManager;

	TwTerminate();

	HDRPipeline::GetInstance()->Destroy();
}

Engine::~Engine()
{

}

Camera * Engine::GetCamera()
{
	return camera;
}

MeshManager * Engine::GetMeshManager()
{
	return modelManager;
}

void Engine::Render()
{
	HDRPipeline::GetInstance()->Render();
}

bool Engine::Update()
{
	InputManager::GetInstance()->Update();
	camera->Update();

	SceneManager::GetInstance()->Update();

	PhysicsManager::GetInstance()->Update();

	

	if (InputManager::GetInstance()->IsKeyClicked(DIK_C))
	{
		camera->ToggleFPSMovement();
	}

	if (InputManager::GetInstance()->IsKeyClicked(DIK_M))
	{
		InputManager::GetInstance()->ToggleMouseLock();
	}

	if (InputManager::GetInstance()->IsKeyClicked(DIK_ESCAPE))
	{
		return false;
	}

	return true;
}

void Engine::LoadLevel(void)
{
	string saveFilePath = StringUtility::GetContentPath();
	saveFilePath.append("TestSave.sav");
	SceneManager::GetInstance()->LoadSceneFromFile(saveFilePath.c_str());
}

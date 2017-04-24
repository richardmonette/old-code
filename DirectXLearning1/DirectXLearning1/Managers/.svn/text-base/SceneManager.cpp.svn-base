#include "../Global.h"

#include <dinput.h>

#include "SceneManager.h"
#include "MeshManager.h"
#include "InputManager.h"
#include "../Objects/GameObject.h"
#include "../Objects/Handle.h"
#include "../Camera.h"
#include "../Engine.h"
#include "../Physics/PhysicsManager.h"
#include <AntTweakBar.h>
#include "../Utility/StringUtility.h"

SceneManager* SceneManager::pInstance = NULL;

SceneManager* SceneManager::GetInstance()
{	
	if (!pInstance)
	{
		pInstance = new SceneManager;
	}

	return pInstance;
}


SceneManager::SceneManager()
{
	m_Selected = NULL;
}

void SceneManager::Initialize(LPDIRECT3DDEVICE9 direct3DDevice, MeshManager *meshManager)
{
	m_Direct3DDevice = direct3DDevice;
	m_meshManager = meshManager;
}

void SceneManager::Deinitialize()
{
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects[i];
	}
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::Update()
{
	if (m_Selected != NULL)
	{
		NxVec3 selectedPos = m_Selected->GetPosition();

		m_XHandle->SetPosition(&(selectedPos + NxVec3(2, 0, 0)));
		m_YHandle->SetPosition(&(selectedPos + NxVec3(0, 2, 0)));
		m_ZHandle->SetPosition(&(selectedPos + NxVec3(0, 0, -2)));
	}

	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Update();
	}
}

void SceneManager::MouseUpObjects()
{
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->HandleMouseUp();
	}
}

void SceneManager::SetSelectedObject(GameObject *gameObject)
{
	if (gameObject == m_XHandle) return;

	if (gameObject == m_YHandle) return;

	if (gameObject == m_ZHandle) return;

	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->SetSelected(false);
	}

	m_XHandle->SetTargetObject(gameObject);
	m_YHandle->SetTargetObject(gameObject);
	m_ZHandle->SetTargetObject(gameObject);

	gameObject->SetSelected(true);
	m_Selected = gameObject;
}

GameObject * SceneManager::AddGameObject(const char *filename, D3DXVECTOR3 position, const char *name, bool isStatic)
{
	GameObject *gameObject = new GameObject(position, m_Direct3DDevice, name);

	string fullpath = StringUtility::GetContentPath();
	fullpath.append(filename);

	LPD3DXBUFFER bufShipMaterial;
	DWORD numMaterials = 0;
	LPD3DXMESH mesh = m_meshManager->GetMesh(fullpath.c_str(), bufShipMaterial, numMaterials);

	D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufShipMaterial->GetBufferPointer();

	gameObject->SetMesh(filename, mesh, isStatic);

	for(unsigned int i = 0; i < numMaterials; i++)
	{
		D3DMATERIAL9* material = &tempMaterials[i].MatD3D;
		material->Ambient = material->Diffuse;

		gameObject->AddMaterial(material);

		LPDIRECT3DTEXTURE9 texture;

		string diffusePath = StringUtility::GetContentPath();
		string normalPath = StringUtility::GetContentPath();

		if (tempMaterials[i].pTextureFilename != NULL)
		{
			diffusePath.append(tempMaterials[i].pTextureFilename); 
			normalPath.append(tempMaterials[i].pTextureFilename); 
		}

		diffusePath.insert(diffusePath.size()-4, "Diffuse");

		if (SUCCEEDED(D3DXCreateTextureFromFileA(m_Direct3DDevice, diffusePath.c_str(), &texture)))
		{
			gameObject->AddTexture(texture);
		}

		normalPath.insert(normalPath.size()-4, "Normal");

		if (SUCCEEDED(D3DXCreateTextureFromFileA(m_Direct3DDevice, normalPath.c_str(), &texture)))
		{
			gameObject->AddTexture(texture);
		}
	}

	AddGameObject(gameObject);

	return gameObject;
}

void SceneManager::AddGameObject(GameObject *gameObject)
{
	m_GameObjects.push_back(gameObject);
}

void SceneManager::RenderGameObjects()
{
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

GameObject * SceneManager::PickGameObject()
{
	Camera *camera = Engine::GetInstance()->GetCamera();

	D3DXVECTOR2 cursorPosition;
	InputManager::GetInstance()->GetCursorPosition(cursorPosition);

	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * cursorPosition.x ) / SCREEN_WIDTH  ) - 1 ) / camera->GetProjectionMatrix()->_11;
	v.y = -( ( ( 2.0f * cursorPosition.y ) / SCREEN_HEIGHT ) - 1 ) / camera->GetProjectionMatrix()->_22;
	v.z =  1.0f;

	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin, rayDir;

	D3DXMatrixInverse( &m, NULL, camera->GetViewMatrix() );

	// Transform the screen space pick ray into 3D space
	rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	BOOL hasHit;
	float distanceToCollision;
	float currentBestDistance = 99999;
	GameObject *foundObject = NULL;
	
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		// Use inverse of matrix
		D3DXMATRIX matNormal;
		NxVec3 pos = m_GameObjects[i]->GetPosition();
		D3DXMatrixTranslation(&matNormal, pos.x, pos.y, pos.z);
		D3DXMATRIX matInverse;
		D3DXMatrixInverse(&matInverse, NULL, &matNormal);

		// Transform ray origin and direction by inv matrix
		D3DXVECTOR3 rayObjOrigin, rayObjDirection;

		D3DXVec3TransformCoord(&rayObjOrigin, &rayOrigin, &matInverse);
		D3DXVec3TransformNormal(&rayObjDirection, &rayDir, &matInverse);
		D3DXVec3Normalize(&rayObjDirection, &rayObjDirection);

		D3DXIntersect(m_GameObjects[i]->GetMesh(), &rayObjOrigin, &rayObjDirection, &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL);

		if (hasHit == TRUE && distanceToCollision < currentBestDistance)
		{
			foundObject = m_GameObjects[i];
		}
	}
	
	return foundObject;
}

void SceneManager::SaveSceneToFile(const char *filename)
{
	ofstream myfile;
	myfile.open (filename);

	NxVec3 position;

	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		position = m_GameObjects[i]->GetPosition();

		myfile << m_GameObjects[i]->GetMeshFilename() << " " << position.x << " " << position.y << " " << position.z << "\n"; 
	}

	myfile.close();
}

void StringExplode(string str, string separator, vector<string>* results){
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos){
		if(found > 0){
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0){
		results->push_back(str);
	}
}

void SceneManager::CreateHandles()
{
	m_XHandle = new Handle(D3DXVECTOR3(0,5,0), m_Direct3DDevice, "xHandle", XAXIS);
	m_XHandle->SetSelectable(false);

	LPD3DXBUFFER materialBuffer;
	DWORD numMaterials = 0;

	LPDIRECT3DTEXTURE9 texture;

	string xHandleFullpath = StringUtility::GetContentPath();
	xHandleFullpath.append("xHandle.x");
	LPD3DXMESH mesh = m_meshManager->GetMesh(xHandleFullpath.c_str(), materialBuffer, numMaterials);
	m_XHandle->SetMesh(xHandleFullpath.c_str(), mesh, true);

	m_YHandle = new Handle(D3DXVECTOR3(0,5,0), m_Direct3DDevice, "yHandle", YAXIS);
	m_YHandle->SetSelectable(false);

	string yHandleFullpath = StringUtility::GetContentPath();
	yHandleFullpath.append("yHandle.x");
	mesh = m_meshManager->GetMesh(yHandleFullpath.c_str(), materialBuffer, numMaterials);
	m_YHandle->SetMesh(yHandleFullpath.c_str(), mesh, true);

	m_ZHandle = new Handle(D3DXVECTOR3(0,5,0), m_Direct3DDevice, "zHandle", ZAXIS);
	m_ZHandle->SetSelectable(false);

	string zHandleFullpath = StringUtility::GetContentPath();
	zHandleFullpath.append("zHandle.x");
	mesh = m_meshManager->GetMesh(zHandleFullpath.c_str(), materialBuffer, numMaterials);
	m_ZHandle->SetMesh(zHandleFullpath.c_str(), mesh, true);

	D3DXCreateTextureFromFileA(m_Direct3DDevice, "C:\\Documents and Settings\\Administrator\\My Documents\\Visual Studio 2008\\Projects\\DirectXLearning1\\DirectXLearning1\\Content\\textures\\DefaultDiffuse.png", &texture);
	m_XHandle->AddTexture(texture);
	m_YHandle->AddTexture(texture);
	m_ZHandle->AddTexture(texture);
	D3DXCreateTextureFromFileA(m_Direct3DDevice, "C:\\Documents and Settings\\Administrator\\My Documents\\Visual Studio 2008\\Projects\\DirectXLearning1\\DirectXLearning1\\Content\\textures\\DefaultNormal.png", &texture);
	m_XHandle->AddTexture(texture);
	m_YHandle->AddTexture(texture);
	m_ZHandle->AddTexture(texture);

	AddGameObject(m_XHandle);
	AddGameObject(m_YHandle);
	AddGameObject(m_ZHandle);
}

void SceneManager::LoadSceneFromFile(const char *filename)
{
	//WARNING: Make sure this clears out the scene properly before loading another
	Deinitialize();

	string line;
	ifstream myfile (filename);
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			getline (myfile, line);

			vector<string> components;
			StringExplode(line, " ", &components);

			if (components.size() > 0)
			{
				D3DXVECTOR3 position;
				position.x = (float)atof( components[2].c_str() );
				position.y = (float)atof( components[3].c_str() );
				position.z = (float)atof( components[4].c_str() );

				bool isStatic = (components[5].compare("static") == 0);

				AddGameObject(components[0].c_str(), position, components[1].c_str(), isStatic);
			}
		}
		myfile.close();
	}
	else
	{
		MessageBox(NULL, "Scene Could Not Be Loaded", "Error", MB_OK);
	}

	CreateHandles();

	ostringstream oss;
	oss << "Loaded " << m_GameObjects.size() << " GameObjects.\n";

	OutputDebugStr(oss.str().c_str());
}
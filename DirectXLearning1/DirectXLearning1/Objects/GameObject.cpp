#define NOMINMAX // for PhysX

#include "GameObject.h"
#include "../Engine.h"
#include "../Camera.h"
#include "../Render/HDRPipeline.h"

GameObject::GameObject(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 direct3DDevice, const char *name)
{
	m_Position = position;

	m_Direct3DDevice = direct3DDevice;
	m_Selected = false;
	m_Selectable = true;

	m_Name = name;
}

GameObject::~GameObject()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		if (m_Textures[i] != NULL)
		{
			m_Textures[i]->Release();
		}
	}
}

//virtual
void GameObject::Update()
{

}

void GameObject::Draw()
{
	D3DXMATRIX matRotX, matRotY, matRotZ;

	NxVec3 position = m_Actor->getGlobalPosition();
	NxMat33 rotMat = m_Actor->getGlobalOrientation();

	float zRot = NxMath::atan2(rotMat.getRow(1).x,rotMat.getRow(0).x);
	float yRot = -NxMath::asin(rotMat.getRow(2).x);
	float xRot = NxMath::atan2(rotMat.getRow(2).y,rotMat.getRow(2).z);

	D3DXMatrixRotationX( &matRotX, xRot );
	D3DXMatrixRotationY( &matRotY, yRot );
	D3DXMatrixRotationZ( &matRotZ, zRot ); 

	D3DXMatrixTranslation(&m_PositionMatrix, position.x, position.y, position.z);

	D3DXMATRIX world = ( matRotX * matRotY * matRotZ ) * m_PositionMatrix;

	D3DXMATRIX *view = Engine::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX *projection = Engine::GetInstance()->GetCamera()->GetProjectionMatrix();

	D3DXMATRIX wvpMatrix = world * (*view) * (*projection);

	HDRPipeline *pipeline = HDRPipeline::GetInstance();

	pipeline->GetVertexShaderConstantTable()->SetMatrix( "World", &world );
	pipeline->GetVertexShaderConstantTable()->SetMatrix( "View", view );
	pipeline->GetVertexShaderConstantTable()->SetMatrix( "Projection", projection );

	D3DXVECTOR4 tint(1, 1, 1, 1);

	if (m_Selected)
	{
		tint.x = 3;
		tint.y = 0;
		tint.z = 0;
		tint.w = 0;
	}

	pipeline->GetVertexShaderConstantTable()->SetVector("SelectedTint", &tint);

	// HACK
	LPDIRECT3DTEXTURE9 diffuseTexture = GetTexture(0);
	LPDIRECT3DTEXTURE9 normalTexture = GetTexture(1);

	pipeline->GetVertexShaderConstantTable()->SetTexture("diffuseTexture", diffuseTexture);
	pipeline->GetVertexShaderConstantTable()->SetTexture("NormalMap", normalTexture);
	
	pipeline->GetVertexShaderConstantTable()->CommitChanges();

	GetMesh()->DrawSubset(0);

	//TODO: Make this optional
	//m_ModelBoundingBox.Draw(m_Direct3DDevice);
}

void GameObject::AddMaterial(D3DMATERIAL9 * material)
{
	m_Materials.push_back(material);
}

void GameObject::AddTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_Textures.push_back(texture);
}

void GameObject::SetSelectable(bool selectable)
{
	m_Selectable = selectable;
}

bool GameObject::GetSelectable()
{
	return m_Selectable;
}

void GameObject::SetSelected(bool selected)
{
	m_Selected = selected;
}

bool GameObject::GetSelected()
{
	return m_Selected;
}

NxVec3 GameObject::GetPosition()
{
	return m_Actor->getGlobalPosition();
}

void GameObject::SetPosition(NxVec3 *position)
{
	m_Actor->setGlobalPosition(*position);
}

D3DMATERIAL9 * GameObject::GetMaterial(unsigned int index)
{
	return m_Materials[index];
}

LPDIRECT3DTEXTURE9 GameObject::GetTexture(unsigned int index)
{
	if (index < m_Textures.size())
	{
		return m_Textures[index];
	}
	else
	{
		return NULL;
	}
}

unsigned int GameObject::GetNumMaterials()
{
	return m_Materials.size();
}

const char * GameObject::GetMeshFilename()
{
	return m_MeshFilename.c_str();
}

const char * GameObject::GetName()
{
	return m_Name.c_str();
}

LPD3DXMESH GameObject::GetMesh()
{
	return m_Mesh;
}

void GameObject::SetMesh(const char *filename, LPD3DXMESH mesh, bool isStatic)
{
	m_MeshFilename = filename;
	m_Mesh = mesh;

	CreateAABB();

	D3DXVECTOR3 *min = m_ModelBoundingBox.GetMinBounds();
	D3DXVECTOR3 *max = m_ModelBoundingBox.GetMaxBounds();

	float width =  fabs(max->x - min->x) / 2.0f;
	float height = fabs(max->y - min->y) / 2.0f;
	float depth =  fabs(max->z - min->z) / 2.0f;

	NxVec3 dimensions(width, height, depth);

	m_Actor = PhysicsManager::GetInstance()->CreateBox(&dimensions, isStatic);
	m_Actor->setGlobalPosition(NxVec3(m_Position.x, m_Position.y, m_Position.z));
}

//virtual
void GameObject::HandleMouseDown()
{
}

//virtual
void GameObject::HandleMouseUp()
{
}

void GameObject::CreateAABB()
{
	D3DXVECTOR3 *pVertices = NULL;
	
	m_Mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);

	D3DXVECTOR3 minBounds;
	D3DXVECTOR3 maxBounds;

	DWORD numVertices = m_Mesh->GetNumVertices();

	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	m_Mesh->GetDeclaration(decl);
	UINT stride = D3DXGetDeclVertexSize(decl, 0);

	D3DXComputeBoundingBox(pVertices, numVertices, stride, &minBounds, &maxBounds);

	m_ModelBoundingBox.SetMinMax(minBounds, maxBounds, m_Direct3DDevice);

	m_Mesh->UnlockVertexBuffer();
}



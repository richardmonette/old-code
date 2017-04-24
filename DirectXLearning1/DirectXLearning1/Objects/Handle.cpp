#include "Handle.h"
#include "../Managers/InputManager.h"

Handle::Handle(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 direct3DDevice, const char *name, unsigned int axis)
: GameObject(position, direct3DDevice, name)
, m_Pressed(false)
, m_Axis(axis)
{
}

Handle::~Handle()
{
}

//virtual 
void Handle::Update()
{
	D3DXVECTOR2 cursorDelta;
	InputManager::GetInstance()->GetCursorDelta(cursorDelta);

	if (m_Pressed && m_targetObject != NULL)
	{
		NxVec3 position = m_targetObject->GetPosition();
		
		if (m_Axis == XAXIS)
		{
			position.x += 0.1f * cursorDelta.x;
		}
		else if (m_Axis == YAXIS)
		{
			position.y += 0.1f * cursorDelta.x;
		}
		else if (m_Axis == ZAXIS)
		{
			position.z += 0.1f * cursorDelta.x;
		}

		ostringstream oss;
		oss << "newPos: " << position.x << ", " << position.y << ", " << position.z << "\n";

		OutputDebugStr(oss.str().c_str());

		m_targetObject->SetPosition(&position);
	}
}

//virtual
void Handle::HandleMouseDown()
{
	m_Pressed = true;
}

void Handle::HandleMouseUp()
{
	m_Pressed = false;
}

GameObject * Handle::GetTargetObject()
{
	return m_targetObject;
}

void Handle::SetTargetObject(GameObject *gameObject)
{
	m_targetObject = gameObject;
}

#pragma once

#include "../Global.h"
#include "GameObject.h"

#include <d3d9.h>
#include <d3dx9.h>

class Handle : public GameObject
{
public:
	Handle(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 direct3DDevice, const char *name, unsigned int axis);
	virtual ~Handle();

	virtual void Update();

	GameObject * GetTargetObject();
	void SetTargetObject(GameObject *gameObject);

	virtual void HandleMouseDown();
	virtual void HandleMouseUp();

private:

	D3DXVECTOR3 previousPosition;
	bool m_Pressed;
	unsigned int m_Axis;

	GameObject *m_targetObject;

};
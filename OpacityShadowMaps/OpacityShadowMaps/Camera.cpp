
#include "Camera.h"

static const float MOUSE_SPEED = 0.1f;
static const float INVERT_Y_AXIS = 1.0f;

Camera::Camera()
{
	m_Position = D3DXVECTOR3 (6.0f, 7.0f, 6.0f);

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Roll = 0.0f;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DXToRadian(45), 1.0f, 1.0f, 100.0f);

	D3DXMatrixTranslation(&m_WorldMatrix, 0.0f, 0.0f, 0.0f);
}

Camera::Camera( const D3DXVECTOR3* position, const D3DXVECTOR3* orientation )
{
	m_Position = *position;
	m_TransformedForward = *orientation;

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Roll = 0.0f;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DXToRadian(45), 1.0f, 1.0f, 100.0f);

	D3DXMatrixTranslation(&m_WorldMatrix, 0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{
}

// Depreciated, now that cameras follow objects?
void Camera::Update(float mouse_delta_x, float mouse_delta_y, bool w_down, bool s_down, bool d_down, bool a_down)
{
	m_Yaw += mouse_delta_x * MOUSE_SPEED;
	m_Pitch += mouse_delta_y * MOUSE_SPEED * INVERT_Y_AXIS;

	const D3DXVECTOR3 forwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMATRIX yawPitchRoll;
	D3DXMatrixRotationYawPitchRoll(&yawPitchRoll, D3DXToRadian(m_Yaw), D3DXToRadian(m_Pitch), D3DXToRadian(m_Roll));

	D3DXVECTOR4 out;
	D3DXVec3Transform(&out, &forwardVector, &yawPitchRoll);

	m_TransformedForward = D3DXVECTOR3 (out.x, out.y, out.z);

	out.x += m_Position.x;
	out.y += m_Position.y;
	out.z += m_Position.z;

	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &D3DXVECTOR3 (out.x, out.y, out.z), &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));

	if (w_down)
	{
		D3DXVECTOR3 normalized;
		D3DXVec3Normalize(&normalized, &m_TransformedForward);
		m_Position += normalized * 1;
	}
	else if (s_down)
	{
		D3DXVECTOR3 normalized;
		D3DXVec3Normalize(&normalized, &m_TransformedForward);
		m_Position -= normalized * 1;
	}

	if (d_down)
	{
		//D3DXVECTOR3 right;
		D3DXVec3Cross(&m_Right, &m_TransformedForward, &upVector);
		m_Position -= m_Right * 1;
	}
	else if (a_down)
	{
		D3DXVECTOR3 right;
		D3DXVec3Cross(&right, &m_TransformedForward, &upVector);
		m_Position += right * 1;
	}
}

void Camera::Update( const D3DXVECTOR3* position, const D3DXVECTOR3* orientation )
{
	D3DXVECTOR3 out;

	out.x = orientation->x + position->x;
	out.y = orientation->y + position->y;
	out.z = orientation->z + position->z;
	m_TransformedForward = *orientation;
	m_Position = *position;

	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &D3DXVECTOR3 (out.x, out.y, out.z), &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));
}

D3DXVECTOR3 * Camera::GetForwardVector()
{
	return &m_TransformedForward;
}

D3DXVECTOR3 * Camera::GetCameraPosition()
{
	return &m_Position;
}

D3DXMATRIX * Camera::GetWorldMatrix()
{
	return &m_WorldMatrix;
}

D3DXMATRIX * Camera::GetViewMatrix()
{
	return &m_ViewMatrix;
}

D3DXMATRIX * Camera::GetProjectionMatrix()
{
	return &m_ProjectionMatrix;
}

#include "Camera.h"
#include "Managers/InputManager.h"

static const float MOUSE_SPEED = 0.1f;
static const float INVERT_Y_AXIS = 1.0f;

Camera::Camera()
{
	m_Position = D3DXVECTOR3 (0.0f, 8.0f, -25.0f);

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;
	m_Roll = 0.0f;

	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, D3DXToRadian(45), 1.33f, 1.0f, 1000.0f);

	D3DXMatrixTranslation(&m_WorldMatrix, 0.0f, 0.0f, 0.0f);

	m_IsUsingFPSMovement = true;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (m_IsUsingFPSMovement)
	{
		DIMOUSESTATE *mouseState = InputManager::GetInstance()->GetMouseState();

		m_Yaw += mouseState->lX * MOUSE_SPEED;
		m_Pitch += mouseState->lY * MOUSE_SPEED * INVERT_Y_AXIS;

		const D3DXVECTOR3 forwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		const D3DXVECTOR3 upVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXMATRIX yawPitchRoll;
		D3DXMatrixRotationYawPitchRoll(&yawPitchRoll,D3DXToRadian(m_Yaw),D3DXToRadian(m_Pitch),D3DXToRadian(m_Roll));

		D3DXVECTOR4 out;
		D3DXVec3Transform(&out, &forwardVector, &yawPitchRoll);

		m_TransformedForward = D3DXVECTOR3 (out.x, out.y, out.z);

		out.x += m_Position.x;
		out.y += m_Position.y;
		out.z += m_Position.z;

		D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &D3DXVECTOR3 (out.x, out.y, out.z), &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));

		if (InputManager::GetInstance()->IsKeyPressed(DIK_W))
		{
			D3DXVECTOR3 normalized;
			D3DXVec3Normalize(&normalized, &m_TransformedForward);
			m_Position += normalized;
		}
		else if (InputManager::GetInstance()->IsKeyPressed(DIK_S))
		{
			D3DXVECTOR3 normalized;
			D3DXVec3Normalize(&normalized, &m_TransformedForward);
			m_Position -= normalized;
		}

		if (InputManager::GetInstance()->IsKeyPressed(DIK_D))
		{
			D3DXVECTOR3 right;
			D3DXVec3Cross(&right, &m_TransformedForward, &upVector);
			m_Position -= right;
		}
		else if (InputManager::GetInstance()->IsKeyPressed(DIK_A))
		{
			D3DXVECTOR3 right;
			D3DXVec3Cross(&right, &m_TransformedForward, &upVector);
			m_Position += right;
		}
	}
}

void Camera::ToggleFPSMovement()
{
	m_IsUsingFPSMovement = !m_IsUsingFPSMovement;
}

bool Camera::GetFPSMovement()
{
	return m_IsUsingFPSMovement;
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

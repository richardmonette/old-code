#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Update();

	void ToggleFPSMovement();
	bool GetFPSMovement();

	D3DXMATRIX * GetWorldMatrix();
	D3DXMATRIX * GetViewMatrix();
	D3DXMATRIX * GetProjectionMatrix();

	D3DXVECTOR3 * GetForwardVector();
	D3DXVECTOR3 * GetCameraPosition();

private:

	D3DXMATRIX m_WorldMatrix;
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	D3DXVECTOR3 m_TransformedForward;

	float m_Yaw;
	float m_Pitch;
	float m_Roll;

	D3DXVECTOR3 m_Position;

	bool m_IsUsingFPSMovement;
};

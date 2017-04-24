#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

class Camera
{
public:
	Camera();
	Camera( const D3DXVECTOR3* position, const D3DXVECTOR3* orientation );
	virtual ~Camera();

	void Update(float mouse_delta_x, float mouse_delta_y, bool w_down, bool s_down, bool d_down, bool a_down);
	void Update( const D3DXVECTOR3 * position, const D3DXVECTOR3 * orientation );

	D3DXMATRIX * GetWorldMatrix();
	D3DXMATRIX * GetViewMatrix();
	D3DXMATRIX * GetProjectionMatrix();

	D3DXVECTOR3 * GetForwardVector();
	D3DXVECTOR3 * GetRightVector() { return &m_Right; };
	D3DXVECTOR3 * GetCameraPosition();

private:

	D3DXMATRIX m_WorldMatrix;
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	D3DXVECTOR3 m_TransformedForward;
	D3DXVECTOR3 m_Right;
	D3DXVECTOR3 m_Position;

	float m_Yaw;
	float m_Pitch;
	float m_Roll;
};

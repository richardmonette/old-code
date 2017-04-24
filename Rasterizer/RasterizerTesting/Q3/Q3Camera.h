#pragma once

#include "../Math3D.h"

class Q3Camera 
{

public:

	Q3Camera();	

	Vec3 Position() {	return m_vPosition;		}
	Vec3 View()		{	return m_vView;			}
	Vec3 UpVector() {	return m_vUpVector;		}
	Vec3 Strafe()	{	return m_vStrafe;		}
	
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	void RotateView(float angle, float X, float Y, float Z);
	void SetViewByMouse(int x, int y); 
	void RotateAroundPoint(Vec3 vCenter, float X, float Y, float Z);
	void StrafeCamera(float speed);
	void MoveCamera(float speed);
	void Update();
	void Look();

private:

	Vec3 m_vPosition;					
	Vec3 m_vView;						
	Vec3 m_vUpVector;		
	Vec3 m_vStrafe;						
};

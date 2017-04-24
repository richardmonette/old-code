#include "Q3Camera.h"
#include "../Renderer.h"

#define kSpeed	200.0f

Q3Camera::Q3Camera()
{
	Vec3 vZero = Vec3(0.0, 0.0, 0.0);
	Vec3 vView = Vec3(0.0, 1.0, 0.5);
	Vec3 vUp   = Vec3(0.0, 0.0, 1.0);

	m_vPosition	= vZero;
	m_vView		= vView;
	m_vUpVector	= vUp;
}

void Q3Camera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	Vec3 vPosition	= Vec3(positionX, positionY, positionZ);
	Vec3 vView		= Vec3(viewX, viewY, viewZ);
	Vec3 vUpVector	= Vec3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView     = vView;
	m_vUpVector = vUpVector;
}

void Q3Camera::SetViewByMouse(int x, int y)
{
	int middleX = resX / 2.0f;
	int middleY = resY / 2.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;						

	angleY = (float)( (middleX - x) ) / 500.0f;		
	angleZ = (float)( (middleY - y) ) / 500.0f;		

	currentRotX -= angleZ;  

	if(currentRotX > 1.0f)
		currentRotX = 1.0f;
	else if(currentRotX < -1.0f)
		currentRotX = -1.0f;
	else
	{
		Vec3 vAxis = cross(m_vView - m_vPosition, m_vUpVector);
		normalizeSelf(vAxis);

		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
		RotateView(angleY, 0, 1, 0);
	}
}

void Q3Camera::RotateView(float angle, float x, float y, float z)
{
	Vec3 vNewView;
	Vec3 vView = m_vView - m_vPosition;		

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	m_vView = m_vPosition + vNewView;
}

void Q3Camera::StrafeCamera(float speed)
{	
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

void Q3Camera::MoveCamera(float speed)
{
	Vec3 vVector = m_vView - m_vPosition;
	normalizeSelf(vVector);

	m_vPosition.x += vVector.x * speed;
	m_vPosition.y += vVector.y * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.y += vVector.y * speed;
	m_vView.z += vVector.z * speed;
}

void Q3Camera::Update() 
{
	Vec3 vCross = cross(m_vView - m_vPosition, m_vUpVector);

	normalizeSelf(vCross);

	m_vStrafe = vCross;

	SetViewByMouse(0,0);
}

void Q3Camera::Look()
{
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

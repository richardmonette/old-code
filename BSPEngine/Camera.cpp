#include "Main.h"
#include "Camera.h"
#include "Quake3Bsp.h"
#include "Mat4x4.h"
#include <iostream>

static const Vector3 zero = Vector3(0.0, 0.0, 0.0);
static const Vector3 unit_z = Vector3(0.0, 0.0, 1.0);
static const Vector3 unit_y = Vector3(0.0, 1.0, 0.0);
static const Vector3 unit_x = Vector3(1.0, 0.0, 0.0);

Camera::Camera()
{
	view = unit_z;
	up = unit_y;
	right = unit_x;

	canJump = true;
	yVelocity = 0.0f;

	controllerHitReport = new ControllerHitReport();

	rotY = 0;
	rotX = 0;

	freelook = true;
}

NxController * Camera::SetPhysicsController(NxController *c)
{
	controller = c;
	controllerHitReport->RegisterCamera(this);

	return controller;
}

void Camera::PositionCamera(float positionX, float positionY, float positionZ,
				  		    float viewX,     float viewY,     float viewZ,
							float upVectorX, float upVectorY, float upVectorZ)
{
	Vector3 p = Vector3(positionX, positionY, positionZ);
	Vector3 v = Vector3(viewX, viewY, viewZ);
	Vector3 u = Vector3(upVectorX, upVectorY, upVectorZ);

	SetPosition(p);
	view = v;
	up = u;
}

inline float ToRadians(const float degrees)
{	
	return degrees * (3.14 / 180.0);
}

void Camera::SetViewByMouse(float x, float y)
{
	rotY += x;	
	rotX += y;

	Mat4x4 rotMatrix;
	CreateMatrixFromAngles(&rotMatrix, ToRadians(rotX), ToRadians(rotY), 0.0);

	view = unit_z * rotMatrix;
	right = unit_x * rotMatrix;
	up = unit_y * rotMatrix;

	view.Normalize();
	right.Normalize();
	up.Normalize();
}

void Camera::StrafeCamera(float speed)
{	
	xVelocity += right.x * speed;
	zVelocity += right.z * speed;
}

void Camera::MoveCamera(float speed)
{
	xVelocity += view.x * speed;
	zVelocity += view.z * speed;
}

inline float clamp(float x, const float a, const float b)
{
    return x < a ? a : (x > b ? b : x);
}

void Camera::SetFrameShift(const float x, const float y, const float z)
{
	frameShift.x = x;// * 0.5 + frameShift.x * 0.5; // Interpolate a bit to smooth this out
	frameShift.y = y;
	frameShift.z = z;// * 0.5 + frameShift.z * 0.5;
}

void Camera::CheckForMovement(bool forwardDown, bool backwardDown, bool leftDown, bool rightDown)
{	
	fly.GetNextCameraPostion(flypos, flylook);

	const float speed = 1.0f;

	NxVec3 move(0,0,0);

	yVelocity -= 0.1f;

	if (canJump) { xVelocity *= 0.9f; } else { xVelocity *= 0.95f; }

	xVelocity = clamp(xVelocity, -2.0, 2.0);

	if (fabs(xVelocity) < 0.001f)
	{
		xVelocity = 0.0f;
	}

	if (canJump) { zVelocity *= 0.9f; } else { yVelocity *= 0.95f; }

	zVelocity = clamp(zVelocity, -2.0, 2.0);

	if (fabs(zVelocity) < 0.001f)
	{
		zVelocity = 0.0f;
	}

	if (fabs(frameShift.y) > 0)
	{
		yVelocity = 0.f;
	}

	move.x += xVelocity;
	move.y += yVelocity;
	move.z += zVelocity;

	move.x += frameShift.x;
	move.y += frameShift.y;
	move.z += frameShift.z;

	if(forwardDown) 
	{				
		MoveCamera(speed);
	}

	if(backwardDown) 
	{			
		MoveCamera(-speed);
	}

	if(leftDown) 
	{			
		StrafeCamera(speed);
	}

	if(rightDown) 
	{			
		StrafeCamera(-speed);
	}	

	NxU32 collisionFlags;
	controller->move(move, COLLIDABLE_MASK, 0.000001f, collisionFlags);

	if (collisionFlags & NXCC_COLLISION_DOWN) 
	{
		canJump = true;
		yVelocity = 0.0f;
	}

	if (collisionFlags & NXCC_COLLISION_UP)
	{
		yVelocity = 0.0f;
	}

	if (GetKeyState(VK_SPACE) & 0x80)
	{
		if (canJump == true)
		{
			canJump = false;
			yVelocity = 3.0f;
		}
	}
}

Vector3 Camera::GetLook()
{
	const Vector3 p = GetPosition();
	return view + p;
}

void Camera::Look()
{
	Vector3 p;
	Vector3 v;

	if (freelook)
	{
		p = GetPosition();
		v = view + p;
	}
	else // track mode
	{
		p = flypos;
		v = flylook;
	}

	gluLookAt(p.x, p.y, p.z, v.x, v.y, v.z, up.x, up.y, up.z);
}

ControllerHitReport * Camera::GetHitCallback()
{
	return controllerHitReport;
}


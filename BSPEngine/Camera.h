
#pragma once

#include "NxCapsuleController.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Flythrough.h"

class ControllerHitReport;

class Camera 
{
public:

	Camera();	

	Vector3 GetPosition() 
	{	
		if (freelook)
		{
			NxExtendedVec3 pos;
			if (controller)
			{
				pos = controller->getFilteredPosition();
			}

			return Vector3(pos.x, pos.y, pos.z);
		}
		else // track mode
		{
			return flypos;
		}

				
	}

	Vector3 GetLook();

	Vector3 GetView()		{ return view; }
	Vector3 GetUpVector()	{ return up; }
	Vector3 GetStrafe()		{ return right; }

	void SetPosition(const Vector3 p)	
	{	
		if (controller)
		{
			controller->setPosition(NxExtendedVec3(p.x, p.y, p.z));
		}
	}

	void SetView(Vector3 v)			{ view = v; }
	void SetUpVector(Vector3 u)		{ up = u; }
	
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	void SetFrameShift(const float x, const float y, const float z);
	
	void Look();

	ControllerHitReport * GetHitCallback();

	NxController *SetPhysicsController(NxController *c);
	ControllerHitReport *controllerHitReport;

	int fps;

	NxController *controller;
	void CheckForMovement(bool forwardDown, bool backwardDown, bool leftDown, bool rightDown);
	void SetViewByMouse(float x, float y); 
	bool freelook;

private:

	Flythrough fly;
Vector3 flypos, flylook;

	
	void StrafeCamera(float speed);
	void MoveCamera(float speed);


	bool canJump;

	float xVelocity;
	float yVelocity;
	float zVelocity;

	float rotX;
	float rotY;

	Vector3 frameShift;
					
	Vector3 view;						
	Vector3 up;		
	Vector3 right;	
};


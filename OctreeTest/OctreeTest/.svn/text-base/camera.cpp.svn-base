
#include "camera.h"
#include "math.h"
#include "glut.h"

CCamera::CCamera()
{
	Position = Vector3 (0.0, 0.0,	0.0);
	ViewDir = Vector3( 0.0, 0.0, -1.0);
	RightVector = Vector3 (1.0, 0.0, 0.0);
	UpVector = Vector3 (0.0, 1.0, 0.0);

	rotX = 0;
	rotY = 180;

	unit_x = Vector3(1,0,0);
	unit_y = Vector3(0,1,0);
	unit_z = Vector3(0,0,1);
}

void CCamera::Move(Vector3 Direction)
{
	Position = Position + Direction;
}

inline float ToRadians(const float degrees)
{	
	return degrees * (3.14 / 180.0);
}

void CCamera::SetViewByMouse(float x, float y)
{
	rotY += x;
	rotX += y;

	Mat4x4 rotMatrix;
	CreateMatrixFromAngles(&rotMatrix, ToRadians(rotX), ToRadians(rotY), 0.0);

	ViewDir = unit_z * rotMatrix;
	RightVector = unit_x * rotMatrix;
	UpVector = unit_y * rotMatrix;

	ViewDir.makeUnitVector();
	RightVector.makeUnitVector();
	UpVector.makeUnitVector();
}

void CCamera::Render()
{
	Vector3 ViewPoint = Position+ViewDir;

	gluLookAt(	Position.x(), Position.y(), Position.z(),
				ViewPoint.x(), ViewPoint.y(), ViewPoint.z(),
				UpVector.x(), UpVector.y(), UpVector.z());

}

void CCamera::MoveForward( GLfloat Distance )
{
	Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight( GLfloat Distance )
{
	Position = Position + (RightVector*Distance);
}

void CCamera::MoveUpward( GLfloat Distance )
{
	Position = Position + (UpVector*Distance);
}
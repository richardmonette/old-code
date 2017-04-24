#include "Camera.h"

static const Vec3 RIGHT(1.0f, 0.0f, 0.0f);
static const Vec3 UP(0.0f, 1.0f, 0.0f);
static const Vec3 FORWARD(0.0f, 0.0f, 1.0f);

Camera::Camera()
{
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;

	projectionMatrix = CreateProjectionMatrix(ToRad(90.0f), 1.33f, 1.0f, 1000.0f);

	position = Vec3(80, 100, 0); 

	right = Vec3(1.0f, 0.0f, 0.0f);
	up =	Vec3(0.0f, 1.0f, 0.0f);
	front = Vec3(0.0f, 0.0f, 1.0f);

	AddVec3(&lookAt, &position, &front);

	viewMatrix = CreateLookAtMatrix( position, lookAt, up );
}

Camera::~Camera()
{
}

Vec3 Camera::GetViewVector()
{
	return front;
}

void Camera::RotateX(float amount)
{
	rotX += ToRad(amount);

	SetMatrixIdentity(&matRotX);

	RotationMatrixFromAxisAngle(&matRotX, &right, -rotX);
}

void Camera::RotateY(float amount)
{
	rotY += ToRad(amount);

	SetMatrixIdentity(&matRotY);

	RotationMatrixFromAxisAngle(&matRotY, &up, -rotY);
}

void Camera::RotateZ(float amount)
{
	rotZ += -ToRad(amount);

	Mat4x4 matRotZ;

	SetMatrixIdentity(&matRotZ);

	RotationMatrixFromAxisAngle(&matRotZ, &front, rotZ);

	Vec3ByMat4x4(&up, &UP, &matRotZ);
	Vec3ByMat4x4(&right, &RIGHT, &matRotZ);

	Normalize(&up);
	Normalize(&right);
}

void Camera::Update()
{
	Mat4x4 rotation;

	CreateMatrixFromAngles(&rotation, rotX, rotY, 0.0f);

	Vec3ByMat4x4(&front, &FORWARD, &rotation);
	Vec3ByMat4x4(&right, &RIGHT, &rotation);
	Vec3ByMat4x4(&up, &UP, &rotation);

	Normalize(&front);
	Normalize(&right);
	Normalize(&up);

	AddVec3(&lookAt, &position, &front);

	flythrough.GetNextCameraPostion(&position, &lookAt);

	viewMatrix = CreateLookAtMatrix(position, lookAt, up);
}

void Camera::MoveForward()
{
	AddVec3(&position, &position, &front);
}

void Camera::MoveBackward()
{
	SubtractVec3(&position, &position, &front);
}

void Camera::MoveLeft()
{
	SubtractVec3(&position, &position, &right);
}

void Camera::MoveRight()
{
	AddVec3(&position, &position, &right);
}

Mat4x4 * Camera::GetViewMatrix()
{
	return &viewMatrix;
}

Mat4x4 * Camera::GetProjectionMatrix()
{
	return &projectionMatrix;
}
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

	position = Vec3(80, 100, 0);//Vec3(0,200,1000); // 

	right = Vec3(1.0f, 0.0f, 0.0f);
	up =	Vec3(0.0f, 1.0f, 0.0f);
	front = Vec3(0.0f, 0.0f, 1.0f);

	lookAt = position + front;

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

	matRotX.identity();

	RotationMatrixFromAxisAngle(&matRotX, &right, -rotX);
}

void Camera::RotateY(float amount)
{
	rotY += ToRad(amount);

	matRotY.identity();

	RotationMatrixFromAxisAngle(&matRotY, &up, -rotY);
}

void Camera::RotateZ(float amount)
{
	rotZ += -ToRad(amount);

	Mat4x4 matRotZ;

	matRotZ.identity();

	RotationMatrixFromAxisAngle(&matRotZ, &front, rotZ);

	up = UP * matRotZ;
	right = RIGHT * matRotZ;

	normalizeSelfFAST(up);
	normalizeSelfFAST(right);
}

void Camera::Update()
{
	Mat4x4 rotation;

	CreateMatrixFromAngles(&rotation, rotX, rotY, 0.0f);

	front = FORWARD * rotation;
	right = RIGHT * rotation;
	up = UP * rotation;

	normalizeSelfFAST(front);
	normalizeSelfFAST(right);
	normalizeSelfFAST(up);

	lookAt = position + front;

	flythrough.GetNextCameraPostion(position, lookAt);

	viewMatrix = CreateLookAtMatrix(position, lookAt, up);

	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::MoveForward()
{
	position += front * 2.0;
}

void Camera::MoveBackward()
{
	position -= front * 2.0;
}

void Camera::MoveLeft()
{
	position -= right * 2.0;
}

void Camera::MoveRight()
{
	position += right * 2.0;
}

Mat4x4 * Camera::GetViewMatrix()
{
	return &viewMatrix;
}

Mat4x4 * Camera::GetProjectionMatrix()
{
	return &projectionMatrix;
}

Mat4x4 * Camera::GetViewProjectionMatrix()
{
	return &viewProjectionMatrix;
}
#pragma once

#include "Math3D.h"
#include "Flythrough.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update();

	void RotateY(float amount);
	void RotateX(float amount);
	void RotateZ(float amount);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	Vec3 GetViewVector();

	Mat4x4 * GetViewMatrix();
	Mat4x4 * GetProjectionMatrix();

	Vec3 position;

private:

	Flythrough flythrough;

	float rotX;
	float rotY;
	float rotZ;

	Vec3 right;
	Vec3 up;
	Vec3 front;

	Mat4x4 matRotX;
	Mat4x4 matRotY;
	Mat4x4 matRotZ;

	Mat4x4 viewMatrix;
	Mat4x4 projectionMatrix;

	Mat4x4 cameraRotation;

	Vec3 lookAt;
};

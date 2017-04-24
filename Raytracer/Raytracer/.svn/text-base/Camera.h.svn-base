#pragma once

#include "Transform.h"

class Film;
class Sample;
class Ray;

class Camera
{
public:
	Camera::Camera(const Transform &world2cam, float hither, float yon, float sopen, float sclose, Film *film)
	{
		WorldToCamera = world2cam;
		CameraToWorld = WorldToCamera.GetInverse();

		ClipHither = hither;
		ClipYon = yon;

		ShutterOpen = sopen;
		ShutterClose = sclose;

		this->film = film;
	}
	~Camera(void);

	virtual float GenerateRay(const Sample &sample, Ray * ray) const = 0;

	Film *film;

protected:

	Transform WorldToCamera, CameraToWorld;
	float ClipHither, ClipYon;
	float ShutterOpen, ShutterClose;
};

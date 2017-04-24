
#pragma once

#include "Camera.h"
#include "Transform.h"

class Film;

class ProjectiveCamera : public Camera
{
public:
	ProjectiveCamera(const Transform &w2c, const Transform &proj, const float Screen[4], float hither, float yon, float sopen, float sclose, float slensr, float focald, Film *f);

	~ProjectiveCamera(void);

protected:

	float LensRadius, FocalDistance;

	Transform ScreenToRaster, RasterToScreen;

	Transform CameraToScreen, WorldToScreen, RasterToCamera;
};

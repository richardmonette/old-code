#pragma once
#include "ProjectiveCamera.h"
#include "GlobalDefine.h"

#include "Transform.h"
#include "Matrix4x4.h"

class Sample;
class Ray;

Transform Perspective(float fov, float n, float f)
{
	float inv_denom = 1.0f/(f-n);

	Matrix4x4 *perp = new Matrix4x4(1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, f*inv_denom, -f*inv_denom,
									0, 0, 1, 0);
	float invTanAng = 1.f / tanf(Radians(fov) / 2.f);
	return Scale(invTanAng, invTanAng, 1) * Transform(perp);
}

class PerspectiveCamera : public ProjectiveCamera
{
public:
	PerspectiveCamera(const Transform &world2cam, const float Screen[4], float hither, float yon, float sopen, float sclose, float lensr, float focald, float fov, Film *film)
		: ProjectiveCamera(world2cam, Perspective(fov, hither, yon), Screen, hither, yon, sopen, sclose, lensr, focald, film)
	{
	}
	~PerspectiveCamera(void);

	float GenerateRay(const Sample &sample, Ray * ray);
	
};

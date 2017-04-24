#include "ProjectiveCamera.h"
#include "Camera.h"
#include "Transform.h"
#include "Film.h"

ProjectiveCamera::ProjectiveCamera(const Transform &w2c, const Transform &proj, const float Screen[4], float hither, float yon, float sopen, float sclose, float slensr, float focald, Film *f)
: Camera(w2c, hither, yon, sopen, sclose, f)
{
	CameraToScreen = proj;
	WorldToScreen = CameraToScreen * WorldToCamera;

	ScreenToRaster = Scale(float(film->xResolution), float(film->yResolution), 1.f) *
		Scale(1.f / (Screen[1] - Screen[0]), 1.f / (Screen[2] - Screen[3]), 1.f) * 
		Translate(Vector(-Screen[0], -Screen[3], 0.f));
	RasterToScreen = ScreenToRaster.GetInverse();

	RasterToCamera = CameraToScreen.GetInverse() * RasterToScreen;

}

ProjectiveCamera::~ProjectiveCamera(void)
{
}

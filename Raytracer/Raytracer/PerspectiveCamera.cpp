#include "PerspectiveCamera.h"
#include "Point.h"
#include "Sample.h"

PerspectiveCamera::~PerspectiveCamera(void)
{
}

float PerspectiveCamera::GenerateRay(const Sample &sample, Ray * ray)
{
	Point Pras(sample.imageX, sample.imageY, 0);
	Point PCamera;
	RasterToCamera(Pras, &PCamera);

	ray->o = PCamera;
	ray->d = Vector(PCamera.x, PCamera.y, PCamera.z);
	ray->time = Lerp(sample.time, ShutterOpen, ShutterClose);
	
	if (LensRadius > 0.)
	{
		float lensU, lensV;
		ConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
		lensU *= LensRadius;
		lensV *= LensRadius;
		float ft = (FocalDistance - ClipHither) / ray->d.z;
		Point Pfocus = (*ray)(ft);
		ray->o.x += lensU;
		ray->o.y += lensV;
		ray->d = Pfocus - ray->o;
	}
	
	ray->d = Normalize(ray->d);
	ray->mint = 0.;
	ray->maxt = (ClipYon - ClipHither) / ray->d.z;
	CameraToWorld(*ray, ray);

	return 1.f;
}


#include "math3D.h"

#include <math.h>

float ToRad(float degree)
{
	float radian = 0;
	radian = degree * (3.14 / 180.0f);
	return radian;
}

Mat4x4 CreateLookAtMatrix(Vec3 &cameraPosition, Vec3 &cameraTarget, Vec3 &upVector)
{
	Mat4x4 rMat;

	Vec3 zaxis;
	SubtractVec3(&zaxis, &cameraTarget, &cameraPosition);
	Normalize(&zaxis);

	Vec3 xaxis;
	Cross(&xaxis, &upVector, &zaxis);
	Normalize(&xaxis);

	Vec3 yaxis;
	Cross(&yaxis, &zaxis, &xaxis);
	Normalize(&yaxis);

	rMat.m[0][0] = xaxis.x; 					    rMat.m[0][1] = yaxis.x;                       rMat.m[0][2] = zaxis.x;          			    rMat.m[0][3] = 0;
	rMat.m[1][0] = xaxis.y; 				        rMat.m[1][1] = yaxis.y;                       rMat.m[1][2] = zaxis.y;          			    rMat.m[1][3] = 0;
	rMat.m[2][0] = xaxis.z; 					    rMat.m[2][1] = yaxis.z;                       rMat.m[2][2] = zaxis.z;          			    rMat.m[2][3] = 0;
	rMat.m[3][0] = -Dot(&xaxis, &cameraPosition);   rMat.m[3][1] = -Dot(&yaxis, &cameraPosition); rMat.m[3][2] = -Dot(&zaxis, &cameraPosition); rMat.m[3][3] = 1;

	return rMat;
}

Mat4x4 CreateProjectionMatrix(float fov, float aspect, float zn, float zf)
{	
	Mat4x4 rMat;
	
	SetMatrixZero(&rMat);
	
	float h = 1.0f / tanf(fov / 2.0f);
	float w = h / aspect;
	
	rMat.m[0][0] = w;//(2 * zn) / w;
	rMat.m[1][1] = h;//(2 * zn) / h;
	rMat.m[2][2] = zf / ( zf - zn );
	rMat.m[2][3] = zf * zn / ( zf - zn );
	rMat.m[3][2] = -1;
	
	return rMat;
}

Mat4x4 CreateViewportScaleMatrix(int w, int h, float x, float y)
{
	Mat4x4 rMat;

	SetMatrixZero(&rMat);

	rMat.m[0][0] = w;
	rMat.m[1][1] = -h;
	rMat.m[3][0] = x;
	rMat.m[3][1] = h + y; 
	rMat.m[2][2] = 1.0f;
	rMat.m[3][3] = 1.0f;

	return rMat;
}

// Axis should already be normalized!

void RotationMatrixFromAxisAngle(Mat4x4 *outMatrix, Vec3 *axis, float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0 - c;

	outMatrix->m[0][0] = c + axis->x * axis->x * t;
	outMatrix->m[1][1] = c + axis->y * axis->y * t;
	outMatrix->m[2][2] = c + axis->z * axis->z * t;

	float tmp1 = axis->x * axis->y * t;
	float tmp2 = axis->z * s;
	outMatrix->m[1][0] = tmp1 + tmp2;
	outMatrix->m[0][1] = tmp1 - tmp2;
	tmp1 = axis->x * axis->z * t;
	tmp2 = axis->y * s;
	outMatrix->m[2][0] = tmp1 - tmp2;
	outMatrix->m[0][2] = tmp1 + tmp2;    
	tmp1 = axis->y * axis->z * t;
	tmp2 = axis->x * s;
	outMatrix->m[2][1] = tmp1 + tmp2;
	outMatrix->m[1][2] = tmp1 - tmp2;
}

void CreateMatrixFromAngles(Mat4x4 *outMatrix, float x, float y, float z)
{
	float A       = cosf(x);
	float B       = sinf(x);
	float C       = cosf(y);
	float D       = sinf(y);
	float E       = cosf(z);
	float F       = sinf(z);

	float AD      =   A * D;
	float BD      =   B * D;

	outMatrix->m[0][0]  =   C * E;
	outMatrix->m[0][1]  =  -C * F;
	outMatrix->m[0][2]  =  -D;
	outMatrix->m[1][0]  = -BD * E + A * F;
	outMatrix->m[1][1]  =  BD * F + A * E;
	outMatrix->m[1][2]  =  -B * C;
	outMatrix->m[2][0]  =  AD * E + B * F;
	outMatrix->m[2][1]  = -AD * F + B * E;
	outMatrix->m[2][2] =   A * C;

	outMatrix->m[0][3]  =  outMatrix->m[1][3] = outMatrix->m[2][3] = outMatrix->m[3][0] = outMatrix->m[3][1] = outMatrix->m[3][2] = 0;
	outMatrix->m[3][3] =  1;
}
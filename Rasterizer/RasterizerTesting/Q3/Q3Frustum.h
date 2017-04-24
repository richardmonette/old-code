#pragma once

#include "../Math3D.h"

class Q3Frustum 
{

public:

	void CalculateFrustum(Mat4x4 *comboMatrix, bool normalize);
	bool PointInFrustum(float x, float y, float z);
	bool SphereInFrustum(float x, float y, float z, float radius);
	bool CubeInFrustum(float x, float y, float z, float size);
	bool BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

private:

	float m_Frustum[6][4];

};

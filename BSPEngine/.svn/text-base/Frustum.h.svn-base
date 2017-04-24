
#pragma once

#include "main.h"

class Frustum 
{
	public:

		void CalculateFrustum();
		bool PointInFrustum(float x, float y, float z);
		bool SphereInFrustum(float x, float y, float z, float radius);
		bool CubeInFrustum(float x, float y, float z, float size);
		bool BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

	private:

		float m_Frustum[6][4];
};

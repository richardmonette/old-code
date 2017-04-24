#include "Q3Frustum.h"
#include "math.h"

#define FIRST_OBJECT_ID  3								

enum FrustumSide
{
	RIGHT	= 0,
	LEFT	= 1,
	BOTTOM	= 2,
	TOP		= 3,
	BACK	= 4,
	FRONT	= 5
}; 

enum PlaneData
{
	A = 0,
	B = 1,
	C = 2,
	D = 3
};

void NormalizePlane(float frustum[6][4], int side)
{
	float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );

	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}

void Q3Frustum::CalculateFrustum(Mat4x4 *comboMatrix, bool normalize)
{    
	// Left clipping plane
	m_Frustum[LEFT][0] = comboMatrix->m[0][3] + comboMatrix->m[0][0];
	m_Frustum[LEFT][1] = comboMatrix->m[1][3] + comboMatrix->m[1][0];
	m_Frustum[LEFT][2] = comboMatrix->m[2][3] + comboMatrix->m[2][0];
	m_Frustum[LEFT][3] = comboMatrix->m[3][3] + comboMatrix->m[3][0];

	// Right clipping plane
	m_Frustum[RIGHT][0] = comboMatrix->m[0][3] - comboMatrix->m[0][0];
	m_Frustum[RIGHT][1] = comboMatrix->m[1][3] - comboMatrix->m[1][0];
	m_Frustum[RIGHT][2] = comboMatrix->m[2][3] - comboMatrix->m[2][0];
	m_Frustum[RIGHT][3] = comboMatrix->m[3][3] - comboMatrix->m[3][0];

	// Top clipping plane
	m_Frustum[TOP][0] = comboMatrix->m[0][3] - comboMatrix->m[0][1];
	m_Frustum[TOP][1] = comboMatrix->m[1][3] - comboMatrix->m[1][1];
	m_Frustum[TOP][2] = comboMatrix->m[2][3] - comboMatrix->m[2][1];
	m_Frustum[TOP][3] = comboMatrix->m[3][3] - comboMatrix->m[3][1];

	// Bottom clipping plane
	m_Frustum[BOTTOM][0] = comboMatrix->m[0][3] + comboMatrix->m[0][1];
	m_Frustum[BOTTOM][1] = comboMatrix->m[1][3] + comboMatrix->m[1][1];
	m_Frustum[BOTTOM][2] = comboMatrix->m[2][3] + comboMatrix->m[2][1];
	m_Frustum[BOTTOM][3] = comboMatrix->m[3][3] + comboMatrix->m[3][1];

	// Near clipping plane
	m_Frustum[FRONT][0] = comboMatrix->m[0][2];
	m_Frustum[FRONT][1] = comboMatrix->m[1][2];
	m_Frustum[FRONT][2] = comboMatrix->m[2][2];
	m_Frustum[FRONT][3] = comboMatrix->m[3][2];

	// Far clipping plane
	m_Frustum[BACK][0] = comboMatrix->m[0][3] - comboMatrix->m[0][2];
	m_Frustum[BACK][1] = comboMatrix->m[1][3] - comboMatrix->m[1][2];
	m_Frustum[BACK][2] = comboMatrix->m[2][3] - comboMatrix->m[2][2];
	m_Frustum[BACK][3] = comboMatrix->m[3][3] - comboMatrix->m[3][2];

	// Normalize the plane equations, if requested
	if (normalize == true)
	{
		NormalizePlane(m_Frustum, LEFT);
		NormalizePlane(m_Frustum, RIGHT);
		NormalizePlane(m_Frustum, TOP);
		NormalizePlane(m_Frustum, BOTTOM);
		NormalizePlane(m_Frustum, FRONT);
		NormalizePlane(m_Frustum, BACK);
	}
}

bool Q3Frustum::PointInFrustum( float x, float y, float z )
{
	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0)
		{
			return false;
		}
	}

	return true;
}

bool Q3Frustum::SphereInFrustum( float x, float y, float z, float radius )
{
	for(int i = 0; i < 6; i++ )	
	{
		if( m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= -radius )
		{
			return false;
		}
	}
	
	return true;
}

bool Q3Frustum::CubeInFrustum( float x, float y, float z, float size )
{
	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;

		return false;
	}

	return true;
}

bool Q3Frustum::BoxInFrustum( float x, float y, float z, float x2, float y2, float z2)
{
	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y  + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y  + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z  + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y  + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y  + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x  + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;
		if(m_Frustum[i][A] * x2 + m_Frustum[i][B] * y2 + m_Frustum[i][C] * z2 + m_Frustum[i][D] > 0)  continue;

		return false;
	}

	return true;
}


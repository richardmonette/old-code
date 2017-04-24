#pragma once

#include "Matrix4x4.h"
#include "Reference.h"

class Transform
{
public:
	Transform()
	{
		m = mInv = new Matrix4x4;
	}

	Transform(float mat[4][4])
	{
		m = new Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
						  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
						  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
						  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

		mInv = m->Inverse();
	}

	Transform(const Reference<Matrix4x4> &mat, const Reference<Matrix4x4> &minv)
	{
		m = mat;
		mInv = minv;
	}

	Transform GetInverse() const
	{
		return Transform(mInv, m);
	}

	bool SwapHandedness()const 
	{
		float det = ((m->m[0][0] * (m->m[1][2] * m->m[2][2] - m->m[1][2] * m->m[2][1])) - (m->m[0][1] * (m->m[1][0] * m->m[2][2] - m->m[1][2] * m->m[2][0])) + (m->m[0][2] * (m->m[1][0] * m->m[2][1] - m->m[1][1] * m->m[2][0])));

		return det < 0.f;
	}

	~Transform();

private:

	Reference<Matrix4x4> m, mInv;

};

Transform Scale(float x, float y, float z) 
{
	Matrix4x4 *m, *minv;
	m = new Matrix4x4(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
	minv = new Matrix4x4(1.f/x,     0,     0, 0,
		0, 1.f/y,     0, 0,
		0,     0, 1.f/z, 0,
		0,     0,     0, 1);
	return Transform(m, minv);
}

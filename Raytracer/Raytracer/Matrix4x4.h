#pragma once

#include "Reference.h"
#include "ReferenceCounted.h"
#include "math.h"

#include <vector>

using namespace std;

struct Matrix4x4 : public ReferenceCounted
{

	Matrix4x4()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j) m[i][j] = 1.;
				else m[i][j] = 0.;
			}
		}
	}

	Matrix4x4(float mat[4][4]);
	Matrix4x4(float t00, float t01, float t02, float t03,
			  float t10, float t11, float t12, float t13,
			  float t20, float t21, float t22, float t23,
			  float t30, float t31, float t32, float t33);

	Reference<Matrix4x4> Inverse() const {
		int indxc[4], indxr[4];
		int ipiv[4] = { 0, 0, 0, 0 };
		float minv[4][4];
		memcpy(minv, m, 4*4*sizeof(float));
		for (int i = 0; i < 4; i++) {
			int irow = -1, icol = -1;
			float big = 0.;
			// Choose pivot
			for (int j = 0; j < 4; j++) {
				if (ipiv[j] != 1) {
					for (int k = 0; k < 4; k++) {
						if (ipiv[k] == 0) {
							if (fabsf(minv[j][k]) >= big) {
								big = float(fabsf(minv[j][k]));
								irow = j;
								icol = k;
							}
						}
						else if (ipiv[k] > 1)
						{
							//Error("Singular matrix in MatrixInvert");
						}
					}
				}
			}
			++ipiv[icol];
			// Swap rows _irow_ and _icol_ for pivot
			if (irow != icol) {
				for (int k = 0; k < 4; ++k)
					swap(minv[irow][k], minv[icol][k]);
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (minv[icol][icol] == 0.)
			{
				//Error("Singular matrix in MatrixInvert");
			}
			// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
			float pivinv = 1.f / minv[icol][icol];
			minv[icol][icol] = 1.f;
			for (int j = 0; j < 4; j++)
				minv[icol][j] *= pivinv;
			// Subtract this row from others to zero out their columns
			for (int j = 0; j < 4; j++) {
				if (j != icol) {
					float save = minv[j][icol];
					minv[j][icol] = 0;
					for (int k = 0; k < 4; k++)
						minv[j][k] -= minv[icol][k]*save;
				}
			}
		}
		// Swap columns to reflect permutation
		for (int j = 3; j >= 0; j--) {
			if (indxr[j] != indxc[j]) {
				for (int k = 0; k < 4; k++)
					swap(minv[k][indxr[j]], minv[k][indxc[j]]);
			}
		}
		return new Matrix4x4(minv);
	}

	float m[4][4];
};

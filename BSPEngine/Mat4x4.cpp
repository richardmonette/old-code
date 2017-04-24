
#include "Mat4x4.h"
#include "math.h"

Mat4x4::Mat4x4(void) {
	identity();
}

Mat4x4::Mat4x4(float e11, float e12, float e13, float e14,
			   float e21, float e22, float e23, float e24,
			   float e31, float e32, float e33, float e34,
			   float e41, float e42, float e43, float e44) {
	_11 = e11; _12 = e12; _13 = e13; _14 = e14;
	_21 = e21; _22 = e22; _23 = e23; _24 = e24;
	_31 = e31; _32 = e32; _33 = e33; _34 = e34;
	_41 = e41; _42 = e42; _43 = e43; _44 = e44;
}

Mat4x4::Mat4x4(const Mat4x4& mat) {
	_11 = mat._11; _12 = mat._12; _13 = mat._13; _14 = mat._14;
	_21 = mat._21; _22 = mat._22; _23 = mat._23; _24 = mat._24;
	_31 = mat._31; _32 = mat._32; _33 = mat._33; _34 = mat._34;
	_41 = mat._41; _42 = mat._42; _43 = mat._43; _44 = mat._44;		
}

void Mat4x4::set(float e11, float e12, float e13, float e14,
		 		 float e21, float e22, float e23, float e24,
				 float e31, float e32, float e33, float e34,
				 float e41, float e42, float e43, float e44) {
	_11 = e11; _12 = e12; _13 = e13; _14 = e14;
	_21 = e21; _22 = e22; _23 = e23; _24 = e24;
	_31 = e31; _32 = e32; _33 = e33; _34 = e34;
	_41 = e41; _42 = e42; _43 = e43; _44 = e44;
}
	
void Mat4x4::zero(void) {
	_11 = 0; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 0; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 0; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 0;
}

void Mat4x4::identity(void) {
	_11 = 1; _12 = 0; _13 = 0; _14 = 0;
	_21 = 0; _22 = 1; _23 = 0; _24 = 0;
	_31 = 0; _32 = 0; _33 = 1; _34 = 0;
	_41 = 0; _42 = 0; _43 = 0; _44 = 1;
}

void Mat4x4::scale(float sx, float sy, float sz) {
	Mat4x4 temp = *this, matScale;
	matScale._11 = sx;
	matScale._22 = sy;
	matScale._33 = sz;
	*this = matScale * temp;
}
void Mat4x4::translate(float tx, float ty, float tz) {
	Mat4x4 temp = *this, matTrans;
	matTrans._41 = tx;
	matTrans._42 = ty;
	matTrans._43 = tz;
	*this = matTrans * temp;
}

void Mat4x4::rotate(float x, float y, float z) {
	Mat4x4 temp(*this), rotX, rotY, rotZ;
	
	x *= 3.14f / 180.0f;
	y *= 3.14f / 180.0f;
	z *= 3.14f / 180.0f;
	
	//x = x % 360;
	//y = y % 360;
	//z = z % 360;

	//if(x < 0) x += 360;
	//if(y < 0) y += 360;
	//if(z < 0) z += 360;

	rotX._22 = cos(x);
	rotX._23 = sin(x);
	rotX._32 = -sin(x);
	rotX._33 = cos(x);

	rotY._11 = cos(y);
	rotY._13 = -sin(y);
	rotY._31 = sin(y);
	rotY._33 = cos(y);

	rotZ._11 = cos(z);
	rotZ._12 = sin(z);
	rotZ._21 = -sin(z);
	rotZ._22 = cos(z);

	*this = rotX * temp;
	temp = rotY * *this;
	*this = rotZ * temp;
}

Mat4x4& Mat4x4::operator+=(const Mat4x4& mat) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			m[i][j] += mat.m[i][j];
		}
	}
	return *this;
}

Mat4x4& Mat4x4::operator=(const Mat4x4& mat) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			m[i][j] = mat.m[i][j];
		}
	}
	return *this;
}

Mat4x4& Mat4x4::operator-=(const Mat4x4& mat) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			m[i][j] -= mat.m[i][j];
		}
	}
	return *this;
}

Mat4x4& Mat4x4::operator*=(const Mat4x4& mat) {
	float a, b, c, d;
	int i;
	for(i = 0; i < 4;i++) {
		a = m[i][0];
		b = m[i][1];
		c = m[i][2];
		d = m[i][3];

		m[i][0] = a * mat.m[0][0] + b * mat.m[1][0] + c * mat.m[2][0] + d * mat.m[3][0];
		m[i][1] = a * mat.m[0][1] + b * mat.m[1][1] + c * mat.m[2][1] + d * mat.m[3][1];
		m[i][2] = a * mat.m[0][2] + b * mat.m[1][2] + c * mat.m[2][2] + d * mat.m[3][2];
		m[i][3] = a * mat.m[0][3] + b * mat.m[1][3] + c * mat.m[2][3] + d * mat.m[3][3];
	}
	return *this;
}

Mat4x4& Mat4x4::operator*=(float s) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			m[i][j] *= s;
		}
	}
	return *this;
}

Mat4x4 operator+(const Mat4x4& m1, const Mat4x4& m2) {
	Mat4x4 mat;
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			mat.m[i][j] = m1.m[i][j] + m2.m[i][j];		
		}
	}
	return mat;	
}

Mat4x4 operator-(const Mat4x4& m1, const Mat4x4& m2) {
	Mat4x4 mat;
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			mat.m[i][j] = m1.m[i][j] - m2.m[i][j];		
		}
	}
	return mat;	

}

Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2) {
	Mat4x4 mat;
	float a, b, c, d;
	int i;
	for(i = 0; i < 4; i++) {
		a = m1.m[i][0];
		b = m1.m[i][1];
		c = m1.m[i][2];
		d = m1.m[i][3];

		mat.m[i][0] = a * m2.m[0][0] + b * m2.m[1][0] + c * m2.m[2][0] + d * m2.m[3][0];
		mat.m[i][1] = a * m2.m[0][1] + b * m2.m[1][1] + c * m2.m[2][1] + d * m2.m[3][1];
		mat.m[i][2] = a * m2.m[0][2] + b * m2.m[1][2] + c * m2.m[2][2] + d * m2.m[3][2];
		mat.m[i][3] = a * m2.m[0][3] + b * m2.m[1][3] + c * m2.m[2][3] + d * m2.m[3][3];
	}
	return mat;
}

Mat4x4 operator*(const Mat4x4& mat, float s) {
	Mat4x4 rMat;
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			rMat.m[i][j] = mat.m[i][j] * s;
		}	
	}
	return rMat;
}

Mat4x4 operator*(float s, const Mat4x4& mat) {
	return (mat * s);
}

void CreateMatrixFromAngles(Mat4x4 *outMatrix, float x, float y, float z)
{
	float A       = cos(x);
	float B       = sin(x);
	float C       = cos(y);
	float D       = sin(y);
	float E       = cos(z);
	float F       = sin(z);

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
#include "math3D.h"

Vec2::Vec2(void) 
{ 
	x = y = 0.0f;
}

Vec2::Vec2(float vx, float vy) 
{
	x = vx;
	y = vy;
}

Vec2 operator*(const Vec2& vec, float s) 
{
	return Vec2(vec.x * s, vec.y * s);
}

Vec2 operator*(float s, const Vec2& vec) 
{
	return Vec2(vec.x * s, vec.y * s);
}

Vec3::Vec3(void) 
{ 
	x = y = z = 0.0f;
}

Vec3::Vec3(float vx, float vy, float vz) 
{
	x = vx;
	y = vy;
	z = vz;
}

Vec3::Vec3(const Vec3& vec) 
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void Vec3::set(float vx, float vy, float vz) 
{
	x = vx;
	y = vy;
	z = vz;
}

Vec3& Vec3::operator+=(const Vec3& vec) 
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& vec) 
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	
	return *this;
}

Vec3& Vec3::operator*=(float s) 
{
	x *= s;
	y *= s;
	z *= s;
	
	return *this;
}

Vec3& Vec3::operator/=(float s) 
{
	if(s >= -FLT_ERROR && s <= FLT_ERROR) 
	{
		x = 0; y = 0; z = 0;
		return *this;
	}
	float inv = 1.0f / s;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) 
{
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(const Vec3& vec, float s) {
	return Vec3(vec.x * s, vec.y * s, vec.z * s);
}

Vec3 operator*(float s, const Vec3& vec) {
	return Vec3(vec.x * s, vec.y * s, vec.z * s);
}

Vec3 operator/(const Vec3& vec, float s) {
	if(s >= -FLT_ERROR && s <= FLT_ERROR) return Vec3(0, 0, 0);
	float inv = 1.0f / s;
	return Vec3(vec.x * inv, vec.y * inv, vec.z * inv);
}

// Vector by 4x4 matrix

Vec3 operator*(const Vec3& vec, const Mat4x4& mat) {
	return Vec3(vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31,
				vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32,
				vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33);
}

Vec3 operator*(const Mat4x4& mat, const Vec3& vec) {
	return Vec3(vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31,
				vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32,
				vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33);
}

Vec3& Vec3::operator*=(const Mat4x4& mat) 
{
	*this = *this * mat;
	return *this;
}

float len(const Vec3& vec) 
{
	return (float) sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float lenSqr(const Vec3& vec) 
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

void normalizeSelf(Vec3& vec) 
{
	float l = len(vec);
	if(l >= -FLT_ERROR && l <= FLT_ERROR) return;
	float inv = 1.0f / l;
	vec.x *= inv;
	vec.y *= inv;
	vec.z *= inv;
}

Vec3 normalize(const Vec3& vec) {
	float l = len(vec);
	if(l >= -FLT_ERROR && l <= FLT_ERROR) return Vec3(0, 0, 0);
	float inv = 1.0f / l;
	return Vec3(vec.x * inv, vec.y * inv, vec.z * inv);
}

float dot(const Vec3& v1, const Vec3& v2) 
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// WARNING THIS CROSS MIGHT NOT BE WORKING PROPERLY

Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);
}

/////////////////////////////////////////////////////////////
// Vec4
/////////////////////////////////////////////////////////////

Vec4::Vec4(void) 
{ 
	x = y = z = w = 0.0f;
}

Vec4::Vec4(float vx, float vy, float vz, float vw) 
{
	x = vx;
	y = vy;
	z = vz;
	w = vw;
}

Vec4::Vec4(const Vec4& vec) 
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void Vec4::set(float vx, float vy, float vz, float vw) 
{
	x = vx;
	y = vy;
	z = vz;
	w = vw;
}

Vec4& Vec4::operator+=(const Vec4& vec) 
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	
	return *this;
}

Vec4& Vec4::operator-=(const Vec4& vec) 
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	
	return *this;
}

Vec4& Vec4::operator*=(float s) 
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	
	return *this;
}

Vec4& Vec4::operator/=(float s) 
{
	if(s >= -FLT_ERROR && s <= FLT_ERROR) 
	{
		x = 0; y = 0; z = 0; w = 0;
		return *this;
	}
	float inv = 1.0f / s;
	x *= inv;
	y *= inv;
	z *= inv;
	w *= inv;
	return *this;
}

Vec4 operator+(const Vec4& v1, const Vec4& v2) 
{
	return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

Vec4 operator-(const Vec4& v1, const Vec4& v2) {
	return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Vec4 operator*(const Vec4& vec, float s) {
	return Vec4(vec.x * s, vec.y * s, vec.z * s, vec.w * s);
}

Vec4 operator*(float s, const Vec4& vec) {
	return Vec4(vec.x * s, vec.y * s, vec.z * s, vec.w * s);
}

Vec4 operator/(const Vec4& vec, float s) {
	if(s >= -FLT_ERROR && s <= FLT_ERROR) return Vec4(0, 0, 0, 0);
	float inv = 1.0f / s;
	return Vec4(vec.x * inv, vec.y * inv, vec.z * inv, vec.w * inv);
}

// Vector by 4x4 matrix

Vec4 operator*(const Vec4& vec, const Mat4x4& mat) {
	return Vec4(vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31 + vec.w * mat._41,
				vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32 + vec.w * mat._42,
				vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33 + vec.w * mat._43,
				vec.x * mat._14 + vec.y * mat._24 + vec.z * mat._34 + vec.w * mat._44);
}

Vec4 operator*(const Mat4x4& mat, const Vec4& vec) {
	return Vec4(vec.x * mat._11 + vec.y * mat._21 + vec.z * mat._31 + vec.w * mat._41,
				vec.x * mat._12 + vec.y * mat._22 + vec.z * mat._32 + vec.w * mat._42,
				vec.x * mat._13 + vec.y * mat._23 + vec.z * mat._33 + vec.w * mat._43,
				vec.x * mat._14 + vec.y * mat._24 + vec.z * mat._34 + vec.w * mat._44);
}

Vec4& Vec4::operator*=(const Mat4x4& mat) 
{
	*this = *this * mat;
	return *this;
}

float len(const Vec4& vec) 
{
	return (float) sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

float lenSqr(const Vec4& vec) 
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

void normalizeSelf(Vec4& vec) 
{
	float l = len(vec);
	if(l >= -FLT_ERROR && l <= FLT_ERROR) return;
	float inv = 1.0f / l;
	vec.x *= inv;
	vec.y *= inv;
	vec.z *= inv;
	vec.w *= inv;
}

Vec4 normalize(const Vec4& vec) {
	float l = len(vec);
	if(l >= -FLT_ERROR && l <= FLT_ERROR) return Vec4(0, 0, 0, 0);
	float inv = 1.0f / l;
	return Vec4(vec.x * inv, vec.y * inv, vec.z * inv, vec.w * inv);
}

float dot(const Vec4& v1, const Vec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// WARNING THIS CROSS MIGHT NOT BE WORKING PROPERLY

Vec4 cross(const Vec4& v1, const Vec4& v2) {
	return Vec4(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x,
				0);
}

////////////////////////////////////////////////////////////////////////
// Mat4x4
////////////////////////////////////////////////////////////////////////

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

float ToRad(float degree)
{
	float radian = 0;
	radian = degree * (pi / 180.0f);
	return radian;
}

Mat4x4 CreateLookAtMatrix(Vec3 &cameraPosition, Vec3 &cameraTarget, Vec3 &upVector)
{
	Mat4x4 rMat;

	Vec3 zaxis = (cameraTarget - cameraPosition);
	normalizeSelfFAST(zaxis);

	Vec3 xaxis = cross(upVector, zaxis);
	normalizeSelfFAST(xaxis);

	Vec3 yaxis = cross(zaxis, xaxis);
	normalizeSelfFAST(yaxis);

	rMat.m[0][0] = xaxis.x; 					  rMat.m[0][1] = yaxis.x;                     rMat.m[0][2] = zaxis.x;          			  rMat.m[0][3] = 0;
	rMat.m[1][0] = xaxis.y; 				      rMat.m[1][1] = yaxis.y;                     rMat.m[1][2] = zaxis.y;          			  rMat.m[1][3] = 0;
	rMat.m[2][0] = xaxis.z; 					  rMat.m[2][1] = yaxis.z;                     rMat.m[2][2] = zaxis.z;          			  rMat.m[2][3] = 0;
	rMat.m[3][0] = -dot(xaxis, cameraPosition);   rMat.m[3][1] = -dot(yaxis, cameraPosition); rMat.m[3][2] = -dot(zaxis, cameraPosition); rMat.m[3][3] = 1;

	return rMat;
}

Mat4x4 CreateProjectionMatrix(float fov, float aspect, float zn, float zf)
{	
	Mat4x4 rMat;
	
	rMat.set(0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f);
	
	float h = 1.0f / tan(fov / 2.0f);
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

	rMat.set(0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, 0.0f);

	rMat.m[0][0] = w;
	rMat.m[1][1] = -h;
	rMat.m[3][0] = x;
	rMat.m[3][1] = h + y; 
	rMat.m[2][2] = 1.0f;
	rMat.m[3][3] = 1.0f;

	return rMat;
}

float InvSqrt (float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	return x * (1.5f - xhalf * x * x);
}

void normalizeSelfFAST(Vec3& vec)
{
	float is = InvSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if(is >= -FLT_ERROR && is <= FLT_ERROR) return;
	vec.x *= is;
	vec.y *= is;
	vec.z *= is;
}

void normalizeSelfFAST(Vec4& vec)
{
	float is = InvSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if(is >= -FLT_ERROR && is <= FLT_ERROR) return;
	vec.x *= is;
	vec.y *= is;
	vec.z *= is;
	vec.w *= is;
}

// Axis should already be normalized!

void RotationMatrixFromAxisAngle(Mat4x4 *outMatrix, Vec3 *axis, float angle)
{
	float c = cos(angle);
	float s = sin(angle);
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
#pragma once

#include <math.h>

#define FLT_ERROR 0.00001

class Vec2;
class Vec3;
class Vec4;
class Mat4x4;

class Vec2 
{

public:

	union 
	{
		struct 
		{
			float x, y;
		};
		float v[2];
	};

	Vec2(void);
	Vec2(float vx, float vy);

	friend Vec2 operator*(const Vec2& vec, float s);
	friend Vec2 operator*(float s, const Vec2& vec);
};

class Vec3 
{

public:

	union 
	{
		struct 
		{
			float x, y, z;
		};
		float v[3];
	};

	Vec3(void);
	Vec3(float vx, float vy, float vz);
	Vec3(const Vec3& vec);

	void set(float vx, float vy, float vz);

	Vec3& operator+=(const Vec3& vec);
	Vec3& operator-=(const Vec3& vec);
	Vec3& operator*=(float s);
	Vec3& operator/=(float s);	

	friend Vec3 operator+(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator-(const Vec3& v1, const Vec3& v2);
	friend Vec3 operator*(const Vec3& vec, float s);
	friend Vec3 operator*(float s, const Vec3& vec);
	friend Vec3 operator/(const Vec3& vec, float s);

	friend Vec3 operator*(const Vec3& vec, const Mat4x4& mat);
	friend Vec3 operator*(const Mat4x4& mat, const Vec3& vec);
	Vec3& operator*=(const Mat4x4& mat);

	friend float len(const Vec3& vec);
	friend float lenSqr(const Vec3& vec);
	friend void normalizeSelf(Vec3& vec);
	friend void normalizeSelfFAST(Vec3& vec);
	friend Vec3 normalize(const Vec3& vec);
	friend float dot(const Vec3& v1, const Vec3& v2);
	friend Vec3 cross(const Vec3& v1, const Vec3& v2);

/* What happens if we disable the copy constructor?

private:
	Vec3(const Vec3&);
	Vec3& operator=(const Vec3&);
*/

};
 
class Vec4 {

public:

	union {
		struct {
			float x, y, z, w;
		};
		float v[4];
	};

	Vec4(void);
	Vec4(float vx, float vy, float vz, float vw);
	Vec4(const Vec4& vec);

	void set(float vx, float vy, float vz, float vw);

	Vec4& operator+=(const Vec4& vec);
	Vec4& operator-=(const Vec4& vec);
	Vec4& operator*=(float s);
	Vec4& operator/=(float s);	

	friend Vec4 operator+(const Vec4& v1, const Vec4& v2);
	friend Vec4 operator-(const Vec4& v1, const Vec4& v2);
	friend Vec4 operator*(const Vec4& vec, float s);
	friend Vec4 operator*(float s, const Vec4& vec);
	friend Vec4 operator/(const Vec4& vec, float s);

	friend Vec4 operator*(const Vec4& vec, const Mat4x4& mat);
	friend Vec4 operator*(const Mat4x4& mat, const Vec4& vec);
	Vec4& operator*=(const Mat4x4& mat);

	friend float len(const Vec4& vec);
	friend float lenSqr(const Vec4& vec);
	friend void normalizeSelf(Vec4& vec);
	friend void normalizeSelfFAST(Vec4& vec);
	friend Vec4 normalize(const Vec4& vec);
	friend float dot(const Vec4& v1, const Vec4& v2);
	friend Vec4 cross(const Vec4& v1, const Vec4& v2);

};

class Mat4x4 
{

public:

	union 
	{
		struct 
		{
			float _11, _12, _13, _14,
				  _21, _22, _23, _24,
				  _31, _32, _33, _34,
				  _41, _42, _43, _44;
		};
		float m[4][4];
	};

	Mat4x4(void);
	Mat4x4(float e11, float e12, float e13, float e14,
		   float e21, float e22, float e23, float e24,
		   float e31, float e32, float e33, float e34,
		   float e41, float e42, float e43, float e44);
	Mat4x4(const Mat4x4& mat);

	void set(float e11, float e12, float e13, float e14,
			 float e21, float e22, float e23, float e24,
			 float e31, float e32, float e33, float e34,
			 float e41, float e42, float e43, float e44);
	
	void zero(void);
	void identity(void);

	void scale(float sx, float sy, float sz);
	void translate(float tx, float ty, float tz);
	void rotate(float rx, float ry, float rz);	

	Mat4x4& operator=(const Mat4x4& mat);
	Mat4x4& operator+=(const Mat4x4& mat);
	Mat4x4& operator-=(const Mat4x4& mat);
	Mat4x4& operator*=(const Mat4x4& mat);
	Mat4x4& operator*=(float s);	

	friend Mat4x4 operator+(const Mat4x4& m1, const Mat4x4& m2);
	friend Mat4x4 operator-(const Mat4x4& m1, const Mat4x4& m2);
	friend Mat4x4 operator*(const Mat4x4& m1, const Mat4x4& m2);
	friend Mat4x4 operator*(const Mat4x4& mat, float s);
	friend Mat4x4 operator*(float s, const Mat4x4& mat);
};

Mat4x4 CreateViewportScaleMatrix(int w, int h, float x, float y);
Mat4x4 CreateProjectionMatrix(float fov, float aspect, float znear, float zfar);
Mat4x4 CreateLookAtMatrix(Vec3 &cameraPosition, Vec3 &cameraTarget, Vec3 &upVector);
float ToRad(float degree);
void RotationMatrixFromAxisAngle(Mat4x4 *outMatrix, Vec3 *axis, float angle);
void CreateMatrixFromAngles(Mat4x4 *outMatrix, float x, float y, float z);


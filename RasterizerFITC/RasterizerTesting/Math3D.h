#pragma once

#define FLT_ERROR 0.00001

struct Vec2
{
	Vec2()
	{
		x = y = 0.0f;
	}

	float x, y;
};

struct Vec3
{
	Vec3()
	{
		x = y = z = 0.0f;
	}

	Vec3(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	float x, y, z;
};

struct Vec4
{
	Vec4()
	{
		x = y = z = w = 0.0f;
	}

	Vec4(float x_, float y_, float z_, float w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	float x, y, z, w;
};

struct Mat4x4
{
	Mat4x4()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;
	
		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;
	
		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = 1.0f;
		m[2][3] = 0.0f;
	
		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	float m[4][4];
};

inline static void Vec4ByScalar(Vec4 *vec, float scalar)
{
	vec->x *= scalar;
	vec->y *= scalar;
	vec->z *= scalar;
	vec->w *= scalar;
}

inline static void Vec3ByScalar(Vec3 *vec, float scalar)
{
	vec->x *= scalar;
	vec->y *= scalar;
	vec->z *= scalar;
}

inline static void Vec3DivideByScalar(Vec3 *vec, float scalar)
{
	const float inverse = 1.0f  / scalar; // Multiply by reciprocal for speed

	vec->x *= inverse;
	vec->y *= inverse;
	vec->z *= inverse;
}

inline static void AddVec3(Vec3 *out, const Vec3 *a, const Vec3 *b)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
}

inline static void AddVec4(Vec4 *out, const Vec4 *a, const Vec4 *b)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
}

inline static void SubtractVec4(Vec4 *out, const Vec4 *a, const Vec4 *b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
}

inline static void SubtractVec3(Vec3 *out, const Vec3 *a, const Vec3 *b)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
}

inline static void Vec3ByMat4x4(Vec3 *out, const Vec3 *vec, const Mat4x4 *mat)
{
	Vec4 temp;

	temp.x = vec->x * mat->m[0][0] + vec->y * mat->m[1][0] + vec->z * mat->m[2][0];
	temp.y = vec->x * mat->m[0][1] + vec->y * mat->m[1][1] + vec->z * mat->m[2][1];
	temp.z = vec->x * mat->m[0][2] + vec->y * mat->m[1][2] + vec->z * mat->m[2][2];

	out->x = temp.x;
	out->y = temp.y;
	out->z = temp.z;
}

inline static void Vec4ByMat4x4(Vec4 *out, const Vec4 *vec, const Mat4x4 *mat)
{
	Vec4 temp;

	temp.x = vec->x * mat->m[0][0] + vec->y * mat->m[1][0] + vec->z * mat->m[2][0] + vec->w * mat->m[3][0];
	temp.y = vec->x * mat->m[0][1] + vec->y * mat->m[1][1] + vec->z * mat->m[2][1] + vec->w * mat->m[3][1];
	temp.z = vec->x * mat->m[0][2] + vec->y * mat->m[1][2] + vec->z * mat->m[2][2] + vec->w * mat->m[3][2];
	temp.w = vec->x * mat->m[0][3] + vec->y * mat->m[1][3] + vec->z * mat->m[2][3] + vec->w * mat->m[3][3];

	out->x = temp.x;
	out->y = temp.y;
	out->z = temp.z;
	out->w = temp.w;
}

inline static float Dot(const Vec4 *v1, const Vec4 *v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}

inline static float Dot(const Vec3 *v1, const Vec3 *v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

inline static void Cross(Vec3 * out, const Vec3 *v1, const Vec3 *v2)
{	
	Vec4 temp;

	temp.x = v1->y * v2->z - v1->z * v2->y;
	temp.y = v1->z * v2->x - v1->x * v2->z;
	temp.z = v1->x * v2->y - v1->y * v2->x;

	out->x = temp.x;
	out->y = temp.y;
	out->z = temp.z;
}

inline static void Mat4x4ByMat4x4(Mat4x4 *out, const Mat4x4 *m1, const Mat4x4 *m2)
{
	float a, b, c, d;
	int i;

	for(i = 0; i < 4; i++) 
	{
		a = m1->m[i][0];
		b = m1->m[i][1];
		c = m1->m[i][2];
		d = m1->m[i][3];

		out->m[i][0] = a * m2->m[0][0] + b * m2->m[1][0] + c * m2->m[2][0] + d * m2->m[3][0];
		out->m[i][1] = a * m2->m[0][1] + b * m2->m[1][1] + c * m2->m[2][1] + d * m2->m[3][1];
		out->m[i][2] = a * m2->m[0][2] + b * m2->m[1][2] + c * m2->m[2][2] + d * m2->m[3][2];
		out->m[i][3] = a * m2->m[0][3] + b * m2->m[1][3] + c * m2->m[2][3] + d * m2->m[3][3];
	}
}

static float InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i>>1);
	x = *(float*)&i;
	return x * (1.5f - xhalf * x * x);
}

inline static void Normalize(Vec3 *vec)
{
	float is = InvSqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	if(is >= -FLT_ERROR && is <= FLT_ERROR) return;
	vec->x *= is;
	vec->y *= is;
	vec->z *= is;
}

inline static void Normalize(Vec4 *vec)
{
	float is = InvSqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	if(is >= -FLT_ERROR && is <= FLT_ERROR) return;
	vec->x *= is;
	vec->y *= is;
	vec->z *= is;
	vec->w *= is;
}

inline static void SetMatrixIdentity(Mat4x4 *mat)
{
	mat->m[0][0] = 1.0f;
	mat->m[0][1] = 0.0f;
	mat->m[0][2] = 0.0f;
	mat->m[0][3] = 0.0f;
	
	mat->m[1][0] = 0.0f;
	mat->m[1][1] = 1.0f;
	mat->m[1][2] = 0.0f;
	mat->m[1][3] = 0.0f;
	
	mat->m[2][0] = 0.0f;
	mat->m[2][1] = 0.0f;
	mat->m[2][2] = 1.0f;
	mat->m[2][3] = 0.0f;
	
	mat->m[3][0] = 0.0f;
	mat->m[3][1] = 0.0f;
	mat->m[3][2] = 0.0f;
	mat->m[3][3] = 1.0f;
}

inline static void SetMatrixZero(Mat4x4 *mat)
{
	mat->m[0][0] = 0.0f;
	mat->m[0][1] = 0.0f;
	mat->m[0][2] = 0.0f;
	mat->m[0][3] = 0.0f;
	
	mat->m[1][0] = 0.0f;
	mat->m[1][1] = 0.0f;
	mat->m[1][2] = 0.0f;
	mat->m[1][3] = 0.0f;
	
	mat->m[2][0] = 0.0f;
	mat->m[2][1] = 0.0f;
	mat->m[2][2] = 0.0f;
	mat->m[2][3] = 0.0f;
	
	mat->m[3][0] = 0.0f;
	mat->m[3][1] = 0.0f;
	mat->m[3][2] = 0.0f;
	mat->m[3][3] = 0.0f;
}

Mat4x4 CreateViewportScaleMatrix(int w, int h, float x, float y);
Mat4x4 CreateProjectionMatrix(float fov, float aspect, float znear, float zfar);
Mat4x4 CreateLookAtMatrix(Vec3 &cameraPosition, Vec3 &cameraTarget, Vec3 &upVector);
float ToRad(float degree);
void RotationMatrixFromAxisAngle(Mat4x4 *outMatrix, Vec3 *axis, float angle);
void CreateMatrixFromAngles(Mat4x4 *outMatrix, float x, float y, float z);


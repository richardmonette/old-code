
#pragma once

#include <math.h>
#include <algorithm>

using namespace std;

enum Refl_t { DIFFUSE, SPECULAR, REFRACTIVE };

static const double M_PI = 3.141;
static const int MISS = 0;
static const int HIT = 1;
static const int INPRIM = 2;

inline double clamp(double x)
{ 
	return x < 0 ? 0 : x > 1 ? 1 : x; 
}

inline int toInt(double x)
{ 
	return int(pow(clamp(x),1/2.2)*255+.5); 
}

struct Vec 
{
	double x, y, z;

	Vec(double x_=0, double y_=0, double z_=0)
	{ 
		x=x_;
		y=y_;
		z=z_; 
	}

	Vec operator+(const Vec &b) const 
	{ 
		return Vec(x+b.x,y+b.y,z+b.z); 
	}

	Vec operator-(const Vec &b) const 
	{ 
		return Vec(x-b.x,y-b.y,z-b.z); 
	}

	Vec operator*(double b) const 
	{ 
		return Vec(x*b,y*b,z*b); 
	}

	Vec &operator*=(double f)
	{
		x *= f; y *= f; z *= f;
		return *this;
	}

	Vec mult(const Vec &b) const 
	{ 
		return Vec(x*b.x,y*b.y,z*b.z); 
	}

	float magnitude()
	{
		return sqrt(x*x+y*y+z*z);
	}

	Vec& norm()
	{ 
		return *this = *this * (1/sqrt(x*x+y*y+z*z));
	}

	void zero()
	{
		x = 0; y = 0; z = 0;
	}
};

struct Ray 
{ 
	Vec origin, direction;

	Ray(Vec o_, Vec d_) : origin(o_), direction(d_) {} 
};

double inline dot(const Vec a, const Vec b) 
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
} 

Vec inline cross(const Vec a, const Vec b)
{
	return Vec(
			   a.y * b.z - a.z * b.y,
			   a.z * b.x - a.x * b.z,
		       a.x * b.y - a.y * b.x
			  );
}


struct BoundingBox
{
	Vec pMin;
	Vec pMax;

	BoundingBox(Vec _min, Vec _max)
	{ 
		pMin = _min;
		pMax = _max;
	}

	BoundingBox()
	{ 
		pMin = Vec(99999999, 99999999, 99999999);
		pMax = Vec(-99999999, -99999999, -99999999);
	}

	bool Contains(BoundingBox *bbox)
	{
		bool containsMin = ((bbox->pMin.x>=pMin.x) && (bbox->pMin.x<=pMax.x) && (bbox->pMin.y>=pMin.y) && (bbox->pMin.y<=pMax.y) && (bbox->pMin.z>=pMin.z) && (bbox->pMin.z<=pMax.z));
		bool containsMax = ((bbox->pMax.x>=pMin.x) && (bbox->pMax.x<=pMax.x) && (bbox->pMax.y>=pMin.y) && (bbox->pMax.y<=pMax.y) && (bbox->pMax.z>=pMin.z) && (bbox->pMax.z<=pMax.z));

		if (containsMin && containsMax)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Intersects(const Ray &ray)
	{
		double t0 = -99999999; double t1 = 9999999;

		//X

		double invRayDir = 1.0f / ray.direction.x;
		double tNear = (pMin.x - ray.origin.x) * invRayDir;
		double tFar = (pMax.x - ray.origin.x) * invRayDir;

		if (tNear >tFar)
		{
			double temp = tNear;
			tNear = tFar;
			tFar = temp;
		}
		
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;

		if (t0 > t1) return false;
	
		//Y

		invRayDir = 1.0f / ray.direction.y;
		tNear = (pMin.y - ray.origin.y) * invRayDir;
		tFar = (pMax.y - ray.origin.y) * invRayDir;

		if (tNear >tFar)
		{
			double temp = tNear;
			tNear = tFar;
			tFar = temp;
		}
		
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;

		if (t0 > t1) return false;

		//Z

		invRayDir = 1.0f / ray.direction.z;
		tNear = (pMin.z - ray.origin.z) * invRayDir;
		tFar = (pMax.z - ray.origin.z) * invRayDir;

		if (tNear >tFar)
		{
			double temp = tNear;
			tNear = tFar;
			tFar = temp;
		}
		
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;

		if (t0 > t1) return false;

		return true;
	}

	void Expand(Vec *vertex)
	{
		pMin.x = min( pMin.x, vertex->x );
		pMin.y = min( pMin.y, vertex->y );
		pMin.z = min( pMin.z, vertex->z );

		pMax.x = max( pMax.x, vertex->x );
		pMax.y = max( pMax.y, vertex->y );
		pMax.z = max( pMax.z, vertex->z );
	}
};

struct UV
{
	UV(float _u, float _v)
	{ 
		u = _u;
		v = _v;
	}

	double u, v;
};

/*struct Triangle
{
	int id;

	Vec vertices[3];
	UV uvs[3];
	Vec normals[3];

	Vec emission, color;
	Refl_t reflectionType;

	Vec reflectivity;

	BoundingBox boundingBox;

	void GetPositionBarycentric(Vec *position, float u, float v)
	{
		*position = vertices[0] + (vertices[1] - vertices[0]) * u + (vertices[2] - vertices[0]) * v;
	}

	/*void GetNormalBarycentric(Vec *normal, float u, float v)
	{
		*normal = normals[0] + (normals[1] - normals[0]) * u + (normals[2] - normals[0]) * v;
	}*/
/*
	Vec Triangle::getNormal() const
	{
	   return cross(getTangent(), vertices[2] - vertices[1] ).norm();
	}


	Vec Triangle::getTangent() const
	{
	   return (vertices[1] - vertices[0]).norm();
	}

	bool Intersect
	(
		const Vec& rayOrigin,
		const Vec& rayDirection,
		double& hitDistance
	)
	{
		const Vec edge1( vertices[1] - vertices[0] );
		const Vec edge2( vertices[2] - vertices[0] );

		const Vec pvec( cross( rayDirection, edge2 ) );

		const double det = dot(edge1, pvec );

		{
		  static const double EPSILON = 0.000001f;
		  if( (det > -EPSILON) & (det < EPSILON) )
		  {
			 return false;
		  }

		  double inv_det = 1.0f / det;

		  const Vec tvec( rayOrigin - vertices[0] );

		  const float u = dot( tvec, pvec ) * inv_det;
		  if( (u < 0.0f) | (u > 1.0f) )
		  {
			 return false;
		  }

		  const Vec qvec( cross( tvec, edge1 ) );

		  const float v = dot( rayDirection, qvec ) * inv_det;
		  if( (v < 0.0f) | (u + v > 1.0f) )
		  {
			 return false;
		  }

		  hitDistance = dot( edge2, qvec ) * inv_det;
		}

		return hitDistance >= 0.0f;
	}
};*/

struct Material
{
	Vec ambient;
	Vec diffuse;
	Vec specular;
	double refractiveIndex;
};




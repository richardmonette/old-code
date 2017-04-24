
#ifndef _TRIANGLE_H_ 
#define _TRIANGLE_H_

#include "Material.h"
#include "Vector3.h"
#include "Surface.h"
#include "BBox.h"

class Triangle : public Surface 
{
public:

	Triangle();
	~Triangle();

	Triangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Material* mat);   
	Triangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, const Vector2& _u0, const Vector2& _u1, const Vector2& _u2,
		const Vector3& _n0, const Vector3& _n1, const Vector3& _n2, Material* mat);   
   
	virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;

	virtual BBox boundingBox() const;

	virtual bool randomPoint(const Vector3&, const Vector2&, Vector3&, Vector3&, float&, rgb&) const;

	bool generatePhoton(Ray & ray, const Vector2 & seed1, const Vector2 & seed2);

	rgb photonColor() { return  (1.0f/_pdf) * mptr->photonColor(); }

	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector2 u0;
	Vector2 u1;
	Vector2 u2;
	Vector3 n0;
	Vector3 n1;
	Vector3 n2;

	float _pdf;

	Material* mptr;   
};

#endif // _TRIANGLE_H_
